using namespace std;
namespace fnx
{
    raw_model::raw_model(const std::string& name)
        : asset(name)
        , _vbo_data{}
        , _ibo_data{}
    {
    }

    raw_model::raw_model(const std::string& name, const vbo_data_arr_t& vbo_data)
        : asset(name)
        , _vbo_data(vbo_data)
        , _ibo_data()
    {

    }

    raw_model::raw_model(const std::string& name, const float* vbo_data, size_t size)
        : asset(name)
        , _vbo_data(vbo_data, vbo_data + size)
        , _ibo_data {}
    {

    }

    raw_model::raw_model(const std::string& name, const vbo_data_arr_t& vbo_data, const ibo_data_arr_t& ibo_data)
        : asset(name)
        , _vbo_data(vbo_data)
        , _ibo_data(ibo_data)
    {
    }

    raw_model::raw_model(const std::string& name, float left, float top, float width, float height)
        : asset(name)
    {
        const float arr[18] =
        {
            left, top + height, 0.f, left + width, top + height, 0.f, left + width, top, 0.f,
            left + width, top, 0.f, left, top, 0.f, left, top + height, 0.f
        };
        std::copy(begin(arr), end(arr), std::back_inserter(_vbo_data));
    }

    raw_model::~raw_model()
    {
    }

    std::vector<float> raw_model::get_position_data() const
    {
        std::vector<float> mesh;
        uint32_t stride = get_stride();
        mesh.reserve(get_num_vertices());
        const auto& data = get_vertices();
        for (uint32_t i = 0u; i < data.size(); i+=stride)
        {
            mesh.emplace_back(data[i   ]);
            mesh.emplace_back(data[i + 1]);
            mesh.emplace_back(data[i + 2]);
        }
        return mesh;
    }

    namespace
    {
        int get_index_actual(int index, int total, int data_size = 3, int offset = 0)
        {
            if (index == INT_MIN || total == 0)
                return index;
            index = index < 0 ? total - index + offset : index - 1 - offset;	// adjust for negative index, 0 base and object offset
            index *= data_size;	// adjust for size of a data coordinate
            return index;
        }

        void add_data_by_index(const std::vector<float>& in, std::vector<float>& out, int index, int size)
        {
            for (int i = 0; i < size; i++)
            {
                out.emplace_back(in[index + i]);
            }
        }

        void add_data(const fnx::vector3& in, std::vector<float>& out)
        {
            out.emplace_back(in.x);
            out.emplace_back(in.y);
            out.emplace_back(in.z);
        }

        void add_vertex_data(const std::vector<float>& all_vert_coords, const std::vector<float>& all_texture_coords, const std::vector<float>& all_normals, std::vector<float>& vertex_output,
            int vertex_index, int texture_index, int normal_index)
        {
            // add vertex xyz position data
            add_data_by_index(all_vert_coords, vertex_output, vertex_index, 3);

            // add the vertex texture uv value
            if (texture_index != INT_MIN)
            {
                add_data_by_index(all_texture_coords, vertex_output, texture_index, 2);
            }

            // add the normal xyz direction data
            if (normal_index != INT_MIN)
            {
                add_data_by_index(all_normals, vertex_output, normal_index, 3);
            }
        }

        void add_vertex_data(const std::vector<float>& all_vert_coords, const std::vector<float>& all_texture_coords, std::vector<float>& vertex_output,
            int vertex_index, int texture_index, const fnx::vector3& normal)
        {
            // add vertex xyz position data
            add_data_by_index(all_vert_coords, vertex_output, vertex_index, 3);

            // add the vertex texture uv value
            if (texture_index != INT_MIN)
            {
                add_data_by_index(all_texture_coords, vertex_output, texture_index, 2);
            }

            // add the normal xyz direction data
            add_data(normal, vertex_output);
        }
    }
    
    bool parse_vert_line(const std::vector<std::string>& line_split_by_spaces, std::vector<float>& verts)
    {
        if (line_split_by_spaces.size() < 4)
            return false;
        verts.emplace_back(stof(line_split_by_spaces[1]));
        verts.emplace_back(stof(line_split_by_spaces[2]));
        verts.emplace_back(stof(line_split_by_spaces[3]));
        return true;
    }

    bool parse_texture_coord_line(const std::vector<std::string>& line_split_by_spaces, std::vector<float>& vert_texture_coords)
    {
        if (line_split_by_spaces.size() < 3)
            return false;
        vert_texture_coords.emplace_back(stof(line_split_by_spaces[1]));
        vert_texture_coords.emplace_back(stof(line_split_by_spaces[2]));
        return true;
    }

    bool parse_vert_normal_line(const std::vector<std::string>& line_split_by_spaces, std::vector<float>& vert_normals)
    {
        if (line_split_by_spaces.size() < 4)
            return false;
        vert_normals.emplace_back(stof(line_split_by_spaces[1]));
        vert_normals.emplace_back(stof(line_split_by_spaces[2]));
        vert_normals.emplace_back(stof(line_split_by_spaces[3]));
        return true;
    }

    /// @brief Store data into the IBO if there are only vertex indices. If texture coordinates and/or normals are provided, it will populate the array buffer with both packed data.
    int parse_face_line(const std::vector<std::string>& line_split_by_spaces, raw_model& model,
        const std::vector<float>& all_verts, const std::vector<float>& all_text_coords, const std::vector<float>& all_normals,
        std::vector<unsigned short>& vert_indices, std::vector<float>& vert_output,
        unsigned int vertex_offset, unsigned int text_offset, unsigned int normal_offset)
    {
        int ret = 0;
        if (line_split_by_spaces.size() < 4)
            return ret;

        // vert/text/norm
        size_t pos = 0;
        size_t n_pos = 0;
        /*
            face representations:
                f 1 2 3						triangle with only vertex info
                f 1 2 3 4					square with only vertex info
                f 1/1 2/2 3/3				triangle with vertex and texture info
                f 1/1 2/2 3/3 4/4			square with vertex and texture info
                f 1/1/1 2/2/2 3/3/3			triangle with vertex, texture and normal info
                f 1/1/1 2/2/2 3/3/3 4/4/4	square with vertex, texture and normal info
        */

        std::vector<std::string> face_vert_str;

        auto it = line_split_by_spaces.begin() + 1;	// skip first index as it contains the entire string
        auto it_end = line_split_by_spaces.end();
        // loop through each face vertex info tuple
        for (; it != it_end; ++it)
        {
            auto t = split(*it, "/");
            face_vert_str.insert(face_vert_str.end(), t.begin(), t.end());
        }
        switch (face_vert_str.size())
        {
        case 3: 
            // don't update the output, just populate the element buffer (IBO)
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[0])));
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[1])));
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[2])));
            ret = 3;
            break;	// only vertices triangle
        case 4: 
            // don't update the output, just populate the element buffer (IBO)
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[0])));
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[1])));
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[2])));
            
            vert_indices.emplace_back(static_cast<unsigned short>(vert_indices[vert_indices.size() - 3]));	//face_vert_str[0]
            vert_indices.emplace_back(static_cast<unsigned short>(vert_indices[vert_indices.size() - 2]));	//face_vert_str[2]
            vert_indices.emplace_back(static_cast<unsigned short>(stol(face_vert_str[3])));
            ret = 6;
            break;	// only vertices square
        case 6: 
            {
                // point 0
                auto a = stol(face_vert_str[0]);
                auto b = face_vert_str[1] == "" ? INT_MIN : stol(face_vert_str[1]);
                auto c = INT_MIN;

                // point 1
                auto d = stol(face_vert_str[2]);
                auto e = face_vert_str[3] == "" ? INT_MIN : stol(face_vert_str[3]);
                auto f = INT_MIN;

                // point 2
                auto g = stol(face_vert_str[4]);
                auto h = face_vert_str[5] == "" ? INT_MIN : stol(face_vert_str[5]);
                auto i = INT_MIN;

                fnx::vector3 p1, p2, p3, normal1;
                bool indexed_normals{ true };

                a = get_index_actual(a, static_cast<int>(all_verts.size()), 3, vertex_offset);
                d = get_index_actual(d, static_cast<int>(all_verts.size()), 3, vertex_offset);
                g = get_index_actual(g, static_cast<int>(all_verts.size()), 3, vertex_offset);

                b = get_index_actual(b, static_cast<int>(all_text_coords.size()), 2, text_offset);
                e = get_index_actual(e, static_cast<int>(all_text_coords.size()), 2, text_offset);
                h = get_index_actual(h, static_cast<int>(all_text_coords.size()), 2, text_offset);

                c = get_index_actual(c, static_cast<int>(all_normals.size()), 3, normal_offset);
                f = get_index_actual(f, static_cast<int>(all_normals.size()), 3, normal_offset);
                i = get_index_actual(i, static_cast<int>(all_normals.size()), 3, normal_offset);

                update_min_max(model.get_mutable_aabb(), all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[g], all_verts[g + 1], all_verts[g + 2]);

                if (b != INT_MIN && e != INT_MIN && h != INT_MIN)
                {
                    model.set_texture_data_true();
                }

                if (c != INT_MIN && f != INT_MIN && i != INT_MIN)
                {
                    model.set_normal_data_true();
                }
                else
                {
                    // calculate the normals from the vertices
                    p1 = fnx::vector3(all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                    p2 = fnx::vector3(all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                    p3 = fnx::vector3(all_verts[g], all_verts[g + 1], all_verts[g + 2]);
                    normal1 = get_normal(p1, p2, p3);
                    model.set_normal_data_true();
                    indexed_normals = false;
                }

                if (indexed_normals)
                {
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, a, b, c);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, d, e, f);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, g, h, i);
                }
                else
                {
                    add_vertex_data(all_verts, all_text_coords, vert_output, a, b, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, d, e, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, g, h, normal1);
                }
                ret = 3;
            }
            break;	// vertices and texture coords triangle
        case 8: 
            // don't populate the element buffer (IBO) and pack all vertex information into the output array
            // f v/vt v/vt v/vt v/vt
            {
                // point 0
                auto a = stol(face_vert_str[0]);
                auto b = face_vert_str[1] == "" ? INT_MIN : stol(face_vert_str[1]);
                auto c = INT_MIN;

                // point 1
                auto d = stol(face_vert_str[2]);
                auto e = face_vert_str[3] == "" ? INT_MIN : stol(face_vert_str[3]);
                auto f = INT_MIN;

                // point 2
                auto g = stol(face_vert_str[4]);
                auto h = face_vert_str[5] == "" ? INT_MIN : stol(face_vert_str[5]);
                auto i = INT_MIN;

                // point 3
                auto j = stol(face_vert_str[6]);
                auto k = face_vert_str[7] == "" ? INT_MIN : stol(face_vert_str[7]);
                auto l = INT_MIN;

                // calculate normals
                fnx::vector3 p1, p2, p3, p4, normal1, normal2;
                bool indexed_normals{ true };

                a = get_index_actual(a, static_cast<int>(all_verts.size()), 3, vertex_offset);
                d = get_index_actual(d, static_cast<int>(all_verts.size()), 3, vertex_offset);
                g = get_index_actual(g, static_cast<int>(all_verts.size()), 3, vertex_offset);
                j = get_index_actual(j, static_cast<int>(all_verts.size()), 3, vertex_offset);

                b = get_index_actual(b, static_cast<int>(all_text_coords.size()), 2, text_offset);
                e = get_index_actual(e, static_cast<int>(all_text_coords.size()), 2, text_offset);
                h = get_index_actual(h, static_cast<int>(all_text_coords.size()), 2, text_offset);
                k = get_index_actual(k, static_cast<int>(all_text_coords.size()), 2, text_offset);

                c = get_index_actual(c, static_cast<int>(all_normals.size()), 3, normal_offset);
                f = get_index_actual(f, static_cast<int>(all_normals.size()), 3, normal_offset);
                i = get_index_actual(i, static_cast<int>(all_normals.size()), 3, normal_offset);
                l = get_index_actual(l, static_cast<int>(all_normals.size()), 3, normal_offset);

                update_min_max(model.get_mutable_aabb(), all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[g], all_verts[g + 1], all_verts[g + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[j], all_verts[j + 1], all_verts[j + 2]);

                if (b != INT_MIN && e != INT_MIN && h != INT_MIN && k != INT_MIN)
                {					
                    model.set_texture_data_true();
                }

                if (c != INT_MIN && f != INT_MIN && i != INT_MIN && l != INT_MIN)
                {					
                    model.set_normal_data_true();
                }
                else
                {
                    // calculate the normals from the vertices
                    p1 = fnx::vector3(all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                    p2 = fnx::vector3(all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                    p3 = fnx::vector3(all_verts[g], all_verts[g + 1], all_verts[g + 2]);
                    p4 = fnx::vector3(all_verts[j], all_verts[j + 1], all_verts[j + 2]);
                    normal1 = get_normal(p1, p2, p3);
                    normal2 = get_normal(p1, p3, p4);
                    model.set_normal_data_true();
                    indexed_normals = false;
                }

                if (indexed_normals)
                {
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, a, b, c);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, d, e, f);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, g, h, i);

                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, a, b, c);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, g, h, i);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, j, k, l);
                }
                else
                {
                    add_vertex_data(all_verts, all_text_coords, vert_output, a, b, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, d, e, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, g, h, normal1);

                    add_vertex_data(all_verts, all_text_coords, vert_output, a, b, normal2);
                    add_vertex_data(all_verts, all_text_coords, vert_output, g, h, normal2);
                    add_vertex_data(all_verts, all_text_coords, vert_output, j, k, normal2);
                }
                ret = 6;
            }
            break;	// vertices and texture coords square
        case 9: 
            {
                // point 0
                auto a = stol(face_vert_str[0]);
                auto b = face_vert_str[1] == "" ? INT_MIN : stol(face_vert_str[1]);
                auto c = face_vert_str[2] == "" ? INT_MIN : stol(face_vert_str[2]);

                // point 1
                auto d = stol(face_vert_str[3]);
                auto e = face_vert_str[4] == "" ? INT_MIN : stol(face_vert_str[4]);
                auto f = face_vert_str[5] == "" ? INT_MIN : stol(face_vert_str[5]);

                // point 2
                auto g = stol(face_vert_str[6]);
                auto h = face_vert_str[7] == "" ? INT_MIN : stol(face_vert_str[7]);
                auto i = face_vert_str[8] == "" ? INT_MIN : stol(face_vert_str[8]);

                // calculate normals
                fnx::vector3 p1, p2, p3, normal1;
                bool indexed_normals{ true };

                a = get_index_actual(a, static_cast<int>(all_verts.size()), 3, vertex_offset);
                d = get_index_actual(d, static_cast<int>(all_verts.size()), 3, vertex_offset);
                g = get_index_actual(g, static_cast<int>(all_verts.size()), 3, vertex_offset);

                b = get_index_actual(b, static_cast<int>(all_text_coords.size()), 2, text_offset);
                e = get_index_actual(e, static_cast<int>(all_text_coords.size()), 2, text_offset);
                h = get_index_actual(h, static_cast<int>(all_text_coords.size()), 2, text_offset);

                c = get_index_actual(c, static_cast<int>(all_normals.size()), 3, normal_offset);
                f = get_index_actual(f, static_cast<int>(all_normals.size()), 3, normal_offset);
                i = get_index_actual(i, static_cast<int>(all_normals.size()), 3, normal_offset);

                update_min_max(model.get_mutable_aabb(), all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[g], all_verts[g + 1], all_verts[g + 2]);

                if (b != INT_MIN && e != INT_MIN && h != INT_MIN)
                {
                    model.set_texture_data_true();
                }

                if (c != INT_MIN && f != INT_MIN && i != INT_MIN)
                {
                    model.set_normal_data_true();
                }
                else
                {
                    // calculate the normals from the vertices
                    p1 = fnx::vector3(all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                    p2 = fnx::vector3(all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                    p3 = fnx::vector3(all_verts[g], all_verts[g + 1], all_verts[g + 2]);
                    normal1 = get_normal(p1, p2, p3);
                    model.set_normal_data_true();
                    indexed_normals = false;
                }

                if (indexed_normals)
                {
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, a, b, c);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, d, e, f);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, g, h, i);
                }
                else
                {
                    add_vertex_data(all_verts, all_text_coords, vert_output, a, b, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, d, e, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, g, h, normal1);
                }
                ret = 3;
            }
            break;	// vertices, texture coords, and normals triangle
        case 12: 
            // don't populate the element buffer (IBO) and pack all vertex information into the output array
            // f v/vt/vn v/vt/vn v/vt/vn v/vt/vn
            {
                model.set_texture_data_true();
                model.set_normal_data_true();
                // point 0
                auto a = stol(face_vert_str[0]);
                auto b = face_vert_str[1] == "" ? INT_MIN : stol(face_vert_str[1]);
                auto c = face_vert_str[2] == "" ? INT_MIN : stol(face_vert_str[2]);

                // point 1
                auto d = stol(face_vert_str[3]);
                auto e = face_vert_str[4] == "" ? INT_MIN : stol(face_vert_str[4]);
                auto f = face_vert_str[5] == "" ? INT_MIN : stol(face_vert_str[5]);

                // point 2
                auto g = stol(face_vert_str[6]);
                auto h = face_vert_str[7] == "" ? INT_MIN : stol(face_vert_str[7]);
                auto i = face_vert_str[8] == "" ? INT_MIN : stol(face_vert_str[8]);
                
                // point 3
                auto j = stol(face_vert_str[9]);
                auto k = face_vert_str[10] == "" ? INT_MIN : stol(face_vert_str[10]);
                auto l = face_vert_str[11] == "" ? INT_MIN : stol(face_vert_str[11]);

                // calculate normals
                fnx::vector3 p1, p2, p3, p4, normal1, normal2;
                bool indexed_normals{ true };

                a = get_index_actual(a, static_cast<int>(all_verts.size()), 3, vertex_offset);
                d = get_index_actual(d, static_cast<int>(all_verts.size()), 3, vertex_offset);
                g = get_index_actual(g, static_cast<int>(all_verts.size()), 3, vertex_offset);
                j = get_index_actual(j, static_cast<int>(all_verts.size()), 3, vertex_offset);

                b = get_index_actual(b, static_cast<int>(all_text_coords.size()), 2, text_offset);
                e = get_index_actual(e, static_cast<int>(all_text_coords.size()), 2, text_offset);
                h = get_index_actual(h, static_cast<int>(all_text_coords.size()), 2, text_offset);
                k = get_index_actual(k, static_cast<int>(all_text_coords.size()), 2, text_offset);

                c = get_index_actual(c, static_cast<int>(all_normals.size()), 3, normal_offset);
                f = get_index_actual(f, static_cast<int>(all_normals.size()), 3, normal_offset);
                i = get_index_actual(i, static_cast<int>(all_normals.size()), 3, normal_offset);
                l = get_index_actual(l, static_cast<int>(all_normals.size()), 3, normal_offset);

                update_min_max(model.get_mutable_aabb(), all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[g], all_verts[g + 1], all_verts[g + 2]);
                update_min_max(model.get_mutable_aabb(), all_verts[j], all_verts[j + 1], all_verts[j + 2]);

                if (b != INT_MIN && e != INT_MIN && h != INT_MIN && k != INT_MIN)
                {
                    model.set_texture_data_true();
                }

                if (c != INT_MIN && f != INT_MIN && i != INT_MIN && l != INT_MIN)
                {
                    model.set_normal_data_true();
                }
                else
                {
                    // calculate the normals from the vertices
                    p1 = fnx::vector3(all_verts[a], all_verts[a + 1], all_verts[a + 2]);
                    p2 = fnx::vector3(all_verts[d], all_verts[d + 1], all_verts[d + 2]);
                    p3 = fnx::vector3(all_verts[g], all_verts[g + 1], all_verts[g + 2]);
                    p4 = fnx::vector3(all_verts[j], all_verts[j + 1], all_verts[j + 2]);
                    normal1 = get_normal(p1, p2, p3);
                    normal2 = get_normal(p1, p3, p4);
                    model.set_normal_data_true();
                    indexed_normals = false;
                }

                if (indexed_normals)
                {
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, a, b, c);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, d, e, f);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, g, h, i);

                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, a, b, c);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, g, h, i);
                    add_vertex_data(all_verts, all_text_coords, all_normals, vert_output, j, k, l);
                }
                else
                {
                    add_vertex_data(all_verts, all_text_coords, vert_output, a, b, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, d, e, normal1);
                    add_vertex_data(all_verts, all_text_coords, vert_output, g, h, normal1);

                    add_vertex_data(all_verts, all_text_coords, vert_output, a, b, normal2);
                    add_vertex_data(all_verts, all_text_coords, vert_output, g, h, normal2);
                    add_vertex_data(all_verts, all_text_coords, vert_output, j, k, normal2);
                }
                ret = 6;
            }
            break;	// vertices, texture coords, and normals square
        default:
            ret = 0; break;
        }
        return ret;
    }

    void parse_object(std::ifstream& in,					// file stream input
        const std::string& name,							// this object's name
        std::vector<float>& all_vert_coords,				// all parsed vertices
        std::vector<float>& all_texture_coords,			// all parsed texture coordinates
        std::vector<float>& all_normal_coords,				// all parsed normals
        unsigned int& vertex_offset,							// number of vertices already parsed in the obj file
        unsigned int& text_offset,							// number of texture coordinates already parsed in the obj file 
        unsigned int& normal_offset)							// number of normals already parsed in the obj file
    {
        std::string line;
        unsigned int num_vertices = 0u;
        unsigned int num_normals = 0u;
        unsigned int num_text_coords = 0u;

        auto [materials, _1] = singleton<asset_manager<material>>::acquire();
        auto [models, _2] = singleton<asset_manager<raw_model>>::acquire();


        raw_model& raw = *models.get(name);
        auto& vertex_output = raw.get_mutable_vertices();
        auto& vertex_index_output = raw.get_mutable_indices();
        material_info* mat{ nullptr };	// use a pointer in case a material is never defined
        
        // TODO: This is recursive and will create some pretty large memory chunks. Make this multithreaded and pass in an array of lines to each thread.
        // TODO: Reserve total in vertex_output to avoid the reallocation of the vector

        while (getline(in, line))
        {
            if (line.empty() || line[0] == '#')	// empty line or comment
                continue;

            std::vector<std::string> line_split_by_spaces = split(trim(line), " ");

            if (line_split_by_spaces.empty())
                continue;

            if ("v" == line_split_by_spaces[0])
            {
                if (parse_vert_line(line_split_by_spaces, all_vert_coords))
                    num_vertices++;
            }
            else if ("vt" == line_split_by_spaces[0])
            {
                if (parse_texture_coord_line(line_split_by_spaces, all_texture_coords))
                    num_text_coords++;
            }
            else if ("vn" == line_split_by_spaces[0])
            {
                if (parse_vert_normal_line(line_split_by_spaces, all_normal_coords))
                    num_normals++;
            }
            else if ("f" == line_split_by_spaces[0])
            {
                // vert_ will have the file parsed values and the face line will contain the index into that array
                auto vertices = parse_face_line(line_split_by_spaces, raw, all_vert_coords, all_texture_coords, all_normal_coords, vertex_index_output, vertex_output, vertex_offset, text_offset, normal_offset);
                if (mat)
                    mat->_end += vertices;
            }
            else if ("o" == line_split_by_spaces[0])
            {
                if (line_split_by_spaces.size() > 1)
                {
                    FNX_DEBUG(fnx::format_string("found raw model %s", line_split_by_spaces[1]));
                    parse_object(in, line_split_by_spaces[1], all_vert_coords, all_texture_coords, all_normal_coords, vertex_offset, text_offset, normal_offset);
                    //vertex_offset += num_vertices;
                    //text_offset += num_text_coords;
                    //normal_offset += num_normals;
                }
            }
            else if ("mtllib" == line_split_by_spaces[0])
            {
                // materials will need to be parsed on their own
                if (line_split_by_spaces.size() > 1)
                    parse_material_file(line_split_by_spaces[1]);
            }
            else if ("usemtl" == line_split_by_spaces[0])
            {
                // materials can be overriden using a model
                // create a material map for each material to start and end faces
                int start = 0;
                int end = 0;
                if (mat)
                {
                    start = mat->_end;
                    end = mat->_end;
                }
                mat = &raw.get_mutable_material_map().add_material_range(line_split_by_spaces[1], start, end);
            }
        }

        // cleanup for initial parsing that creates a single model that isn't used
        if ("" == name)
        {
            models.release("");
        }
    }

    void parse_model_file(const std::string& file_path)
    {
        auto[materials, _1] = singleton<asset_manager<material>>::acquire();
        auto[models, _2] = singleton<asset_manager<raw_model>>::acquire();
        auto[textures, _3] = singleton<asset_manager<texture>>::acquire();

        std::ifstream in(file_path);
        if (in.good())
        {
            std::vector<float> all_texture_coords;
            std::vector<float> all_normal_coords;
            std::vector<float> all_vert_coords;
            unsigned int vertex_offset{ 0u };
            unsigned int texture_offset{ 0u };
            unsigned int normal_offset{ 0u };
            parse_object(in, "", all_vert_coords, all_texture_coords, all_normal_coords, vertex_offset, texture_offset, normal_offset);			
        }
        else
        {
            in.close();
            FNX_ERROR(fnx::format_string("unable to load model %s", file_path.c_str()));
            throw "resource file missing";
        }
    }

    std::shared_ptr<fnx::raw_model> raw_model_quad()
    {
        static const std::vector<unsigned short> indices
        {
            0, 1, 2, 0, 2, 3
        };
        static const std::vector<float> data
        {
            // reflected???
            -1.f,1.f,0.f,	// top left
            0.f,0.f,		// bottom left
            -1.f,-1.f,0.f,	// bottom left
            0.f,1.f,		// top left
            1.f,-1.f,0.f,	// bottom right
            1.f,1.f,		// top right
            1.f,1.f,0.f,	// top right
            1.f,0.f			// bottom right
        };

        auto a = std::make_shared<fnx::raw_model>("quad", data, indices);
        a->set_texture_data_true();
        return a;
    }

    std::vector<float> create_vbo_data_for_line(const fnx::vector3& start, 
                                                const fnx::vector3& end, 
                                                const fnx::vector3& start_color, 
                                                reactphysics3d::decimal start_alpha, 
                                                const fnx::vector3& end_color,
                                                reactphysics3d::decimal end_alpha)
    {
        std::vector<float> data{
            start.x, start.y, start.z, start_color.x, start_color.y, start_color.z, start_alpha,
            end.x, end.y, end.z, end_color.x, end_color.y, end_color.z, end_alpha };
        return data;
    }

    fnx::raw_model_handle raw_model_line(const fnx::vector3& start, 
                                         const fnx::vector3& end, 
                                         const fnx::vector3& start_color, 
                                         reactphysics3d::decimal start_alpha, 
                                         const fnx::vector3& end_color, 
                                         reactphysics3d::decimal end_alpha)
    {
        auto a = fnx::make_shared_ref<fnx::raw_model>("line", create_vbo_data_for_line(start, end, start_color, start_alpha, end_color, end_alpha));
        a->set_color_data_true();
        a->set_is_line_true();
        return a;
    }
}