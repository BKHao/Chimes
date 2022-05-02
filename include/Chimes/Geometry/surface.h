#pragma once
#include <Chimes/Geometry/triangle.h>
#include <stack>
#include <fstream>
#include <sstream>
#include <ctime>
namespace Chimes
{
    namespace geometry
    {
        //The base class of polygon mesh, which contains a set of vertices and polygons. Both vertices and polygons are represented by arrays of smart pointers.
        template <typename P>
        requires concept_point_operater<P>&& concept_point_normal<P>&& concept_point_ostream<P>&& concept_point_istream<P>&& concept_point_index<P>
        class PolygonMesh : public GeometryObject
        {
        private:
            using Real = typename P::R;
        public:
            //Empty initialization.
            PolygonMesh()
            {

            }
            //Return a string "Polygon Mesh".
            virtual GeometryType Info() const
            {
                return GeometryType::POLYGON_MESH;
            }
            //Save the mesh to path_name;
            virtual bool save(const std::string& path_name) const = 0;
            //Return the number of vertices.
            size_t NumberOfVertices() const
            {
                return _vertices.size();
            }
            Real* Vertex_Data()
            {
                return vertex_data_;
            }
        protected:
            std::vector<std::shared_ptr<P>> vertices_;
            Real* vertex_data_;
        };

        template <typename P>
        class TriangleMesh : public PolygonMesh<P>
        {
        private:
            using Base = PolygonMesh<P>;
            using Tri = Triangle<P>;
            using Real = typename P::R;
        public:
            //Load from file.
            //The data is saved.
            Triangle_Mesh(std::string file)
            {
                load_from_obj(file);
            }
            //Save the mesh to path_name, only *.obj files are currently supported.
            virtual bool save(const std::string& path_name) const
            {
                std::ofstream out(path_name);
                for (size_t i = 0; i < Base::vertices_.size(); ++i)
                {
                    out << "v " << *Base::vertices_[i] << std::endl;
                }
                for (size_t i = 0; i < _triangles.size(); ++i)
                {
                    out << "f " << _triangles[i]->pid(0) + 1 << " " << _triangles[i]->pid(1) + 1 << " " << _triangles[i]->pid(2) + 1 << std::endl;
                }
                out.close();
                return true;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::TRIANGLE_MESH;
            }
            size_t NumberOfFaces() const
            {
                return triangles_.size();
            }
        protected:
            //Load mesh from obj file.
            bool load_from_obj(const std::string& file)
            {
                std::stringstream scream_line;
                std::ifstream in(file);
                Base::vertices_.clear();
                triangles_.clear();
                std::string line;
                while (std::getline(in, line))
                {
                    scream_line.str("");
                    scream_line.clear();
                    scream_line << line;
                    std::string word;
                    scream_line >> word;
                    if (word == "v")
                    {
                        std::shared_ptr<P> p_point = std::make_shared<P>();
                        scream_line >> *p_point;
                        p_point->set_id(Base::_vertices.size());
                        Base::vertices_.push_back(p_point);
                    }
                    else if (word == "f")
                    {
                        size_t id0, id1, idx;
                        scream_line >> id0 >> id1;
                        while (scream_line >> idx)
                        {

                            std::shared_ptr<Tri> p_tri = std::make_shared<Tri>(Base::vertices_[id0 - 1], Base::vertices_[id1 - 1], Base::vertices_[idx - 1]);
                            p_tri->set_id(_triangles.size());
                            triangles_.push_back(p_tri);
                        }
                    }  
                }
                in.close();
                return true;
            }
        protected:
            std::vector<std::shared_ptr<Tri>> triangles_;

        };

    }
}