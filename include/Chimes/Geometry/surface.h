// 09/05/2022 by BKHao in Chimes.
#pragma once
#include <Chimes/Core/template_concept.h>
#include <Chimes/Core/iterator_base.h>
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Geometry/triangle.h>
#include <fstream>

namespace Chimes
{
	namespace geometry
	{
        template <typename P>
        class VertexIterator : public IteratorBase<P>
        {
        public:
            VertexIterator(): vertex_(nullptr)
            {

            }
            VertexIterator(const VertexIterator& vi) :vertex_(vi.vertex_)
            {

            }
            VertexIterator(VertexIterator&& vi) :vertex_(vi.vertex_)
            {

            }
            VertexIterator(P* vertex) :vertex_(vertex)
            {
                
            }
            VertexIterator& operator++()
            {
                vertex_++;
                return *this;
            }
            VertexIterator operator++(int)
            {
                VertexIterator tmp = *this;
                vertex_++;
                return tmp;
            }
            virtual P& operator*()
            {
                return *vertex_;
            }
            virtual P* operator->()
            {
                return vertex_;
            }
        private:
            P* vertex_;
        };

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
            // Return the vertex buffer data.
            MemoryPtr<Real> Vertex_Data()
            {
                return vertex_data_;
            }
            // Return the i_th vertex.
            const P& Vertex(size_t i) const
            {
                return vertices_[i];
            }
            VertexIterator<P> begin()
            {
                return VertexIterator<P>(vertices_.data());
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
            TriangleMesh(const std::string& file) : Base(), index_data_(0), triangles_(0)
            {
                load_from_obj(file);
            }
            //Save the mesh to path_name, only *.obj files are currently supported.
            virtual bool save(const std::string& path_name) const
            {
                std::ofstream out(path_name);
                for (size_t i = 0; i < Base::vertices_.size(); ++i)
                {
                    out << "v " << Base::vertices_[i] << std::endl;
                }
                for (size_t i = 0; i < triangles_.size(); ++i)
                {
                    out << "f " << triangles_[i].Pid(0) + 1 << " " << triangles_[i].Pid(1) + 1 << " " << triangles_[i].Pid(2) + 1 << std::endl;
                }
                out.close();
                return true;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::TRIANGLE_MESH;
            }
            //Return the number of triangles.
            size_t NumberOfFaces() const
            {
                return triangles_.size();
            }
            // Return the i_th triangle.
            const Tri& Face(size_t i) const
            {
                return triangles_[i];
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
                            index_xyz.push_back(id0 - 1);
                            index_xyz.push_back(id1 - 1);
                            index_xyz.push_back(idx - 1);
                        }
                    }
                }
                in.close();
                Base::vertex_data_ = MemoryPtr<Real>(points_xyz.size());
                index_data_ = MemoryPtr<size_t>(index_xyz.size());
                Base::vertices_.reserve(points_xyz.size() / 3);
                for (size_t i = 0; i < points_xyz.size(); i += 3)
                {
                    Base::vertex_data_[i] = points_xyz[i];
                    Base::vertex_data_[i + 1] = points_xyz[i + 1];
                    Base::vertex_data_[i + 2] = points_xyz[i + 2];                    
                    Base::vertices_.emplace_back(P(Base::vertex_data_, i));
                }
                triangles_.reserve(index_xyz.size() / 3);
                for (size_t i = 0; i < index_xyz.size(); i += 3)
                {
                    index_data_[i] = index_xyz[i];
                    index_data_[i + 1] = index_xyz[i + 1];
                    index_data_[i + 2] = index_xyz[i + 2];
                    std::vector<P> tri_points(3, 0);
                    tri_points[0].share(Base::vertices_[index_data_[i]]);
                    tri_points[1].share(Base::vertices_[index_data_[i + 1]]);
                    tri_points[2].share(Base::vertices_[index_data_[i + 2]]);
                    triangles_.emplace_back(Tri(std::move(tri_points)));
                    triangles_.back().SetPid(index_data_, i);
                }                
                return true;
            }
        protected:
            MemoryPtr<size_t> index_data_;
            std::vector<Tri> triangles_;
        };
	} // namespace geometry
} // namespace Chimes
