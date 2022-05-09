// 09/05/2022 by BKHao in Chimes.
#pragma once
#include <Chimes/Core/template_concept.h>
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Geometry/triangle.h>

namespace Chimes
{
	namespace geometry
	{
        //The base class of polygon mesh, which contains a set of vertices and polygons. Both vertices and polygons are contiguous storage in memory.
        template <typename P>
            requires concept_point_operater<P>&& concept_point_normal<P>&& concept_point_ostream<P>&& concept_point_istream<P>&& concept_point_index<P>
        class PolygonMesh : public GeometryObject
        {
        private:
            using Real = typename P::R;
        public:
            //Empty initialization.
            PolygonMesh() :vertex_data_(0), vertices_(0)
            {

            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::POLYGON_MESH;
            }
            //Save the mesh to path_name;
            virtual bool save(const std::string& path_name) const = 0;
            //Return the number of vertices.
            size_t NumberOfVertices() const
            {
                return vertices_.size();
            }
            MemoryPtr<Real> Vertex_Data()
            {
                return vertex_data_;
            }
        protected:
            MemoryPtr<Real> vertex_data_;
            std::vector<P> vertices_;
        };
        //The common triangle mesh, which contains a set of vertices and triangles. Both vertices and triangles are contiguous storage in memory
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
            TriangleMesh(const std::string& file) :index_data_(0), triangles_(0)
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
                std::vector<Real> points_xyz;
                std::vector<size_t> index_xyz;
                points_xyz.reserve(100);
                index_xyz.reserve(100);
                while (std::getline(in, line))
                {
                    scream_line.str("");
                    scream_line.clear();
                    scream_line << line;
                    std::string word;
                    scream_line >> word;
                    if (word == "v")
                    {
                        Real x, y, z;
                        scream_line >> x >> y >> z;
                        points_xyz.push_back(x);
                        points_xyz.push_back(y);
                        points_xyz.push_back(z);
                    }
                    else if (word == "f")
                    {
                        size_t id0, id1, idx;
                        scream_line >> id0 >> id1;
                        while (scream_line >> idx)
                        {
                            index_xyz.push_back(id0);
                            index_xyz.push_back(id1);
                            index_xyz.push_back(idx);
                        }
                    }
                }
                Base::vertex_data_ = MemoryPtr<Real>(points_xyz.size());
                index_data_ = MemoryPtr<size_t>(index_xyz.size());
                
                in.close();
                return true;
            }
        protected:
            MemoryPtr<size_t> index_data_;
            std::vector<Tri> triangles_;
        };
	} // namespace geometry
} // namespace Chimes
