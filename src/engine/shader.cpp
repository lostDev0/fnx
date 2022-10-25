#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

static const unsigned int MAX_UNIFORM_LENGTH = 32u;
using uniform_map = std::unordered_map<std::string, GLint>;

namespace fnx
{
    class shader_impl
    {
    public:
        GLuint _program;
        GLuint _shaders[shader_max];
        uniform_map _uniforms;

        inline GLint get_uniform_location(const char* name)
        {
            auto iter = _uniforms.find(name);

            if (iter != _uniforms.end())
            {
                return iter->second;
            }
            else
            {
                return -1;
            }
        }
    };

    void shader::init(const std::string& vertex_str, const std::string& fragment_str)
    {
        _impl->_program = glCreateProgram();
        _impl->_shaders[shader_vertex] = glCreateShader(GL_VERTEX_SHADER);
        _impl->_shaders[shader_fragment] = glCreateShader(GL_FRAGMENT_SHADER);
        const char* const vertex_src = vertex_str.c_str();
        const char* const fragment_src = fragment_str.c_str();
        glShaderSource(_impl->_shaders[shader_vertex], 1, &vertex_src, NULL);
        glShaderSource(_impl->_shaders[shader_fragment], 1, &fragment_src, NULL);
        glCompileShader(_impl->_shaders[shader_vertex]);
        // Check Vertex shader
        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetShaderiv(_impl->_shaders[shader_vertex], GL_COMPILE_STATUS, &Result);
        glGetShaderiv(_impl->_shaders[shader_vertex], GL_INFO_LOG_LENGTH, &InfoLogLength);

        if (InfoLogLength > 0)
        {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(_impl->_shaders[shader_vertex], InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            FNX_ERROR(fnx::format_string("%s", &VertexShaderErrorMessage[0]));
        }

        glCompileShader(_impl->_shaders[shader_fragment]);
        // Check Fragment shader
        glGetShaderiv(_impl->_shaders[shader_fragment], GL_COMPILE_STATUS, &Result);
        glGetShaderiv(_impl->_shaders[shader_fragment], GL_INFO_LOG_LENGTH, &InfoLogLength);

        if (InfoLogLength > 0)
        {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(_impl->_shaders[shader_fragment], InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            FNX_ERROR(fnx::format_string("%s", &FragmentShaderErrorMessage[0]));
        }

        glAttachShader(_impl->_program, _impl->_shaders[shader_vertex]);
        glAttachShader(_impl->_program, _impl->_shaders[shader_fragment]);
        // TODO: need these binds?
        glBindAttribLocation(_impl->_program, 0, "vert");
        glBindAttribLocation(_impl->_program, 1, "vertNormal");
        glBindAttribLocation(_impl->_program, 2, "vertTextCoord");
        glLinkProgram(_impl->_program);
        // Check the program
        glGetProgramiv(_impl->_program, GL_LINK_STATUS, &Result);
        glGetProgramiv(_impl->_program, GL_INFO_LOG_LENGTH, &InfoLogLength);

        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(_impl->_program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            FNX_ERROR(fnx::format_string("%s", &ProgramErrorMessage[0]));
        }

        glValidateProgram(_impl->_program);
        // Check the program
        glGetProgramiv(_impl->_program, GL_VALIDATE_STATUS, &Result);
        glGetProgramiv(_impl->_program, GL_INFO_LOG_LENGTH, &InfoLogLength);

        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(_impl->_program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            FNX_ERROR(fnx::format_string("%s", &ProgramErrorMessage[0]));
        }

        FNX_DEBUG("checking uniforms in vertex shader");
        add_all_uniforms(vertex_str);
        FNX_DEBUG("checking uniforms in fragment shader");
        add_all_uniforms(fragment_str);
        FNX_DEBUG("finished checking uniforms");
    }

    void shader::add_all_uniforms(const std::string& source)
    {
        // parse each line of the source file and populate the uniform data
        string line;
        stringstream ss(source);
        const char uniform[] = "uniform ";
        const char structure[] = "struct ";
        uniform_map uniform_map;

        while (getline(ss, line))
        {
            if (starts_with(line, structure))
            {
                // custom structure is defined within the shader
                // TODO : Record Structure members
            }
            else if (starts_with(line, uniform))
            {
                // parse uniform line, eg uniform vec3 myUniform;
                auto tokens = split(line, " ;");
                auto& type = tokens[1];
                auto& name = tokens[2];
                // TODO : if type is a struct, find all uniforms
                // TODO : if type is a struct and array, find all uniforms
                if (ends_with(name, "]"))
                {
                    // array uniform
                    auto array_tokens = split(name, "[]");
                    auto& array_name = array_tokens[0];
                    auto& array_num = array_tokens[1];
                    auto num = static_cast<unsigned int>(atoi(array_num.c_str()));
                    for (auto i = 0u; i < num; ++i)
                    {
                        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
                        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", array_name.c_str(), i);
                        auto loc = glGetUniformLocation(_impl->_program, buffer);
                        _impl->_uniforms[buffer] = loc;   // set the uniform to the invalid value to protect stepping on valid data

                        if (loc != -1)
                        {
                            // uniform was found in the program so add it
                            FNX_DEBUG(fnx::format_string("found uniform array %s", buffer));
                        }
                        else
                        {
                            FNX_INFO(fnx::format_string("unused uniform array in shader (%s)", buffer));
                        }
                    }
                }
                else
                {
                    // single struct uniform
                    auto loc = glGetUniformLocation(_impl->_program, name.c_str());
                    _impl->_uniforms[name] = loc;   // set the uniform to the invalid value to protect stepping on valid data

                    if (loc != -1)
                    {
                        // uniform was found in the program so add it
                        FNX_DEBUG(fnx::format_string("found uniform %s", name.c_str()));
                    }
                    else
                    {
                        FNX_INFO(fnx::format_string("unused uniform in shader (%s)", name.c_str()));
                    }
                }
            }
        }
    }

    shader::shader(const std::string& name, const std::string& content)
        : fnx::asset(name), _impl(new shader_impl())
    {
        istringstream cs(content);
        stringstream ss[shader_max];
        std::string line;
        auto idx = shader_vertex;

        FNX_DEBUG("loading shader");

        while (getline(cs, line))
        {
            if (line.size() > 0 && line[0] == '#')
            {
                // comment line
                if (line == "#vertex shader")
                {
                    idx = shader_vertex;
                }
                else if (line == "#fragment shader")
                {
                    idx = shader_fragment;
                }
                else
                {
                    ss[idx] << line << endl;
                }
            }
            else
            {
                ss[idx] << line << endl;
            }
        }

        if (ss[shader_vertex].str().empty())
        {
            FNX_ERROR("vertex shader is empty");
        }

        if (ss[shader_fragment].str().empty())
        {
            FNX_ERROR("fragment shader is empty");
        }

        init(ss[shader_vertex].str(), ss[shader_fragment].str());
    }

    shader::shader(const std::string& path)
        : fnx::asset(path), _impl(new shader_impl())
    {
        ifstream stream(path);
        stringstream ss[shader_max];

        if (stream.good())
        {
            FNX_DEBUG(fnx::format_string("loading shader from %s", path.c_str()));
            std::string line;
            auto idx = shader_vertex;

            while (getline(stream, line))
            {
                if (line.size() > 0 && line[0] == '#')
                {
                    // comment line
                    if (line == "#vertex shader")
                    {
                        FNX_DEBUG(fnx::format_string("found vertex shader in %s", path));
                        idx = shader_vertex;
                    }
                    else if (line == "#fragment shader")
                    {
                        FNX_DEBUG(fnx::format_string("found fragment shader in %s", path));
                        idx = shader_fragment;
                    }
                    else
                    {
                        ss[idx] << line << endl;
                    }
                }
                else
                {
                    ss[idx] << line << endl;
                }
            }

            if (ss[shader_vertex].str().empty())
            {
                FNX_ERROR("vertex shader is empty");
            }

            if (ss[shader_fragment].str().empty())
            {
                FNX_ERROR("fragment shader is empty");
            }

            stream.close();
            init(ss[shader_vertex].str(), ss[shader_fragment].str());
        }
        else
        {
            FNX_ERROR(fnx::format_string("unable to load shader %s", path.c_str()));
            throw "resource file missing";
        }
    }

    shader::~shader()
    {
        if (nullptr != _impl)
        {
            glDetachShader(_impl->_program, _impl->_shaders[shader_vertex]);
            glDetachShader(_impl->_program, _impl->_shaders[shader_fragment]);
            glDeleteProgram(_impl->_program);
            delete _impl;
        }

        _impl = nullptr;
    }

    void shader::bind() const
    {
        glUseProgram(_impl->_program);
    }

    void shader::apply_all_uniforms() const
    {
        for (const auto& pair : _impl->_uniforms)
        {
            const auto& uniform = pair.second;
            // TODO
        }
    }

    void shader::apply_uniform(const char* uniform_name, int val) const
    {
        glProgramUniform1i(_impl->_program, _impl->get_uniform_location(uniform_name), val);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int val) const
    {
        glProgramUniform1ui(_impl->_program, _impl->get_uniform_location(uniform_name), val);
    }

    void shader::apply_uniform(const char* uniform_name, float val) const
    {
        glProgramUniform1f(_impl->_program, _impl->get_uniform_location(uniform_name), val);
    }

    void shader::apply_uniform(const char* uniform_name, double val) const
    {
        glProgramUniform1d(_impl->_program, _impl->get_uniform_location(uniform_name), val);
    }

    void shader::apply_uniform(const char* uniform_name, int x, int y) const
    {
        glProgramUniform2i(_impl->_program, _impl->get_uniform_location(uniform_name), x, y);
    }

    void shader::apply_uniform(const char* uniform_name, int x, int y, int z) const
    {
        glProgramUniform3i(_impl->_program, _impl->get_uniform_location(uniform_name), x, y, z);
    }

    void shader::apply_uniform(const char* uniform_name, int x, int y, int z, int w) const
    {
        glProgramUniform4i(_impl->_program, _impl->get_uniform_location(uniform_name), x, y, z, w);
    }

    void shader::apply_uniform(const char* uniform_name, const reactphysics3d::Vector2& val) const
    {
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform2d(_impl->_program, _impl->get_uniform_location(uniform_name), val.x, val.y);
        #else                                   // If we are compiling for single precision
            glProgramUniform2f(_impl->_program, _impl->get_uniform_location(uniform_name), val.x, val.y);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, const reactphysics3d::Vector3& val) const
    {
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform3d(_impl->_program, _impl->get_uniform_location(uniform_name), val.x, val.y, val.z);
        #else                                   // If we are compiling for single precision
            glProgramUniform3f(_impl->_program, _impl->get_uniform_location(uniform_name), val.x, val.y, val.z);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, const reactphysics3d::Quaternion& val) const
    {
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform4d(_impl->_program, _impl->get_uniform_location(uniform_name), val.x, val.y, val.z, val.w);
        #else                                   // If we are compiling for single precision
            glProgramUniform4f(_impl->_program, _impl->get_uniform_location(uniform_name), val.x, val.y, val.z, val.w);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, const fnx::matrix4x4& val) const
    {
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniformMatrix4dv(_impl->_program, _impl->get_uniform_location(uniform_name), 1, GL_FALSE, *val.getAll());
        #else                                   // If we are compiling for single precision
            glProgramUniformMatrix4fv(_impl->_program, _impl->get_uniform_location(uniform_name), 1, GL_FALSE, *val.getAll());
        #endif
    }

    /* array */
    void shader::apply_uniform(const char* uniform_name, unsigned int index, int val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform1i(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, unsigned int val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform1ui(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, float val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform1f(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, double val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform1d(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, int x, int y) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform2i(_impl->_program, _impl->get_uniform_location(buffer), x, y);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, int x, int y, int z) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform3i(_impl->_program, _impl->get_uniform_location(buffer), x, y, z);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, int x, int y, int z, int w) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        glProgramUniform4i(_impl->_program, _impl->get_uniform_location(buffer), x, y, z, w);
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, const reactphysics3d::Vector2& val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform2d(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y);
        #else                                   // If we are compiling for single precision
            glProgramUniform2f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, const reactphysics3d::Vector3& val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform3d(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z);
        #else                                   // If we are compiling for single precision
            glProgramUniform3f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, const reactphysics3d::Quaternion& val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform4d(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z, val.w);
        #else                                   // If we are compiling for single precision
            glProgramUniform4f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z, val.w);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, unsigned int index, const fnx::matrix4x4& val) const
    {
        auto buffer = create_array_name(uniform_name, index).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniformMatrix4dv(_impl->_program, _impl->get_uniform_location(buffer), 1, GL_FALSE, *val.getAll());
        #else                                   // If we are compiling for single precision
            glProgramUniformMatrix4fv(_impl->_program, _impl->get_uniform_location(buffer), 1, GL_FALSE, *val.getAll());
        #endif
    }

    /* structs */
    void shader::apply_uniform(const char* uniform_name, const char* member_name, int val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform1i(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform1ui(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, float val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform1f(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, double val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform1d(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, int x, int y) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform2i(_impl->_program, _impl->get_uniform_location(buffer), x, y);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, int x, int y, int z) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform3i(_impl->_program, _impl->get_uniform_location(buffer), x, y, z);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, int x, int y, int z, int w) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        glProgramUniform4i(_impl->_program, _impl->get_uniform_location(buffer), x, y, z, w);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, const reactphysics3d::Vector2& val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform2d(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y);
        #else                                   // If we are compiling for single precision
            glProgramUniform2f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, const reactphysics3d::Vector3& val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform3d(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z);
        #else                                   // If we are compiling for single precision
            glProgramUniform3f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, const reactphysics3d::Quaternion& val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniform4d(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z, val.w);
        #else                                   // If we are compiling for single precision
            glProgramUniform4f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z, val.w);
        #endif
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, const fnx::matrix4x4& val) const
    {
        auto buffer = create_struct_name(uniform_name, member_name).c_str();
        #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
            glProgramUniformMatrix4dv(_impl->_program, _impl->get_uniform_location(buffer), 1, GL_FALSE, *val.getAll());
        #else                                   // If we are compiling for single precision
            glProgramUniformMatrix4fv(_impl->_program, _impl->get_uniform_location(buffer), 1, GL_FALSE, *val.getAll());
        #endif
    }

    /*********************/

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform1i(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, unsigned int val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform1ui(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, float val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform1f(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, double val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform1d(_impl->_program, _impl->get_uniform_location(buffer), val);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int x, int y) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform2i(_impl->_program, _impl->get_uniform_location(buffer), x, y);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int x, int y, int z) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform3i(_impl->_program, _impl->get_uniform_location(buffer), x, y, z);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int x, int y, int z, int w) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform4i(_impl->_program, _impl->get_uniform_location(buffer), x, y, z, w);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const reactphysics3d::Vector2& val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform2f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const reactphysics3d::Vector3& val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform3f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const reactphysics3d::Quaternion& val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        glProgramUniform4f(_impl->_program, _impl->get_uniform_location(buffer), val.x, val.y, val.z, val.w);
    }

    void shader::apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const fnx::matrix4x4& val) const
    {
        auto buffer = create_struct_array_name(uniform_name, member_name, index).c_str();
        // TODO handle compilation of double vs float
        glProgramUniformMatrix4fv(_impl->_program, _impl->get_uniform_location(buffer), 1, GL_FALSE, *val.getAll());
    }

    /* setters with error checking */

    void shader::set_uniform(const char* uniform_name, float val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform1f(_impl->_program, loc, val);
        }
    }

    void shader::set_uniform(const char* uniform_name, double val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform1d(_impl->_program, loc, val);
        }
    }

    void shader::set_uniform(const char* uniform_name, const reactphysics3d::Vector2& val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform2f(_impl->_program, loc, val.x, val.y);
        }
    }

    void shader::set_uniform(const char* uniform_name, const reactphysics3d::Vector3& val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform3f(_impl->_program, loc, val.x, val.y, val.z);
        }
    }

    void shader::set_uniform(const char* uniform_name, const reactphysics3d::Quaternion& val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform4f(_impl->_program, loc, val.x, val.y, val.z, val.w);
        }
    }

    void shader::set_uniform(const char* uniform_name, const fnx::matrix4x4& val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            #if defined(IS_RP3D_DOUBLE_PRECISION_ENABLED)   // If we are compiling for double precision
                glProgramUniformMatrix4dv(_impl->_program, loc, 1, GL_FALSE, *val.getAll());
            #else                                   // If we are compiling for single precision
                glProgramUniformMatrix4fv(_impl->_program, loc, 1, GL_FALSE, *val.getAll());
            #endif
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, int value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1i(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, float value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1f(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, int x, int y) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform2i(_impl->_program, loc, x, y);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, const reactphysics3d::Vector2& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform2f(_impl->_program, loc, value.x, value.y);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, int x, int y, int z) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform3i(_impl->_program, loc, x, y, z);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, const reactphysics3d::Vector3& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform3f(_impl->_program, loc, value.x, value.y, value.z);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, int x, int y, int z, int w) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform4i(_impl->_program, loc, x, y, z, w);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int index, const reactphysics3d::Quaternion& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d]", uniform_name, index);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform4f(_impl->_program, loc, value.x, value.y, value.z, value.w);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index, float value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1f(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index,
        const reactphysics3d::Vector2& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform2f(_impl->_program, loc, value.x, value.y);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index,
        const reactphysics3d::Vector3& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform3f(_impl->_program, loc, value.x, value.y, value.z);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index,
        const reactphysics3d::Quaternion& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform4f(_impl->_program, loc, value.x, value.y, value.z, value.w);
        }
    }

    void shader::set_uniform(const char* uniform_name, unsigned int val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform1ui(_impl->_program, loc, val);
        }
    }

    void shader::set_uniform(const char* uniform_name, int val) const
    {
        auto loc = glGetUniformLocation(_impl->_program, uniform_name);

        if (loc != -1)
        {
            glProgramUniform1i(_impl->_program, loc, val);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index,
        int value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1i(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index,
        int x, int y) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform2i(_impl->_program, loc, x, y);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int index,
        int x, int y, int z) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s[%d].%s", uniform_name, index, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform3i(_impl->_program, loc, x, y, z);
        }
    }

    /**************/

    void shader::set_uniform(const char* uniform_name, const char* member, int value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1i(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, unsigned int value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1ui(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, int x, int y) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform2i(_impl->_program, loc, x, y);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, int x, int y, int z) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform3i(_impl->_program, loc, x, y, z);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, float value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1f(_impl->_program, loc, value);
        }
    }
    void shader::set_uniform(const char* uniform_name, const char* member, double value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform1d(_impl->_program, loc, value);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, const reactphysics3d::Vector2& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform2f(_impl->_program, loc, value.x, value.y);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, const reactphysics3d::Vector3& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform3f(_impl->_program, loc, value.x, value.y, value.z);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, const reactphysics3d::Quaternion& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            glProgramUniform4f(_impl->_program, loc, value.x, value.y, value.z, value.w);
        }
    }

    void shader::set_uniform(const char* uniform_name, const char* member, const fnx::matrix4x4& value) const
    {
        char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member);
        auto loc = glGetUniformLocation(_impl->_program, buffer);

        if (loc != -1)
        {
            // TODO handle compilation diffs between decimal float and double
            glProgramUniformMatrix4fv(_impl->_program, loc, 1, GL_FALSE, *value.getAll());
        }
    }

    std::string shader::create_array_name(const char* uniform_name, unsigned int index)
    {
        std::string buffer = uniform_name;
        buffer += "[";
        buffer += std::to_string(index);
        buffer += "]";
        return buffer;
    }

    std::string shader::create_struct_name(const char* uniform_name, const char* member_name)
    {
        //char buffer[MAX_UNIFORM_LENGTH] = { 0 };
        //snprintf(buffer, MAX_UNIFORM_LENGTH - 1, "%s.%s", uniform_name, member_name);
        std::string buffer = uniform_name;
        buffer += ".";
        buffer += member_name;
        return buffer;
    }

    std::string shader::create_struct_array_name(const char* uniform_name, const char* member_name, unsigned int index)
    {
        std::string buffer = uniform_name;
        buffer += "[";
        buffer += std::to_string(index);
        buffer += "].";
        buffer += member_name;
        return buffer;
    }

}