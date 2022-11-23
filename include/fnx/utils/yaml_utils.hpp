#pragma once
namespace fnx
{
    inline YAML::Emitter& operator<<(YAML::Emitter& out, const vector2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const vector3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }
}

namespace YAML
{
    template<>
    struct convert<fnx::vector2>
    {
        static Node encode(const fnx::vector2& in)
        {
            Node node;
            node.push_back(in.x);
            node.push_back(in.y);
            return node;
        }

        static bool decode(const Node& node, fnx::vector2& out)
        {
            if(!node.IsSequence() || node.size() != 2)
            {
                return false;
            }

            out.x = node[0].as<fnx::decimal>();
            out.y = node[1].as<fnx::decimal>();
            return true;
        }
    };

    template<>
    struct convert<fnx::vector3>
    {        
        static inline Node encode(const fnx::vector3& in)
        {
            Node node;
            node.push_back(in.x);
            node.push_back(in.y);
            node.push_back(in.z);
            return node;
        }

        static inline bool decode(const Node& node, fnx::vector3& out)
        {
            if(!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            out.x = node[0].as<fnx::decimal>();
            out.y = node[1].as<fnx::decimal>();
            out.z = node[2].as<fnx::decimal>();
            return true;
        }
    };
}