// 09/05/2022 by BKHao in Chimes.
#pragma once
#include <Chimes/Core/template_concept.h>
#include <Chimes/Core/iterator_base.h>
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Geometry/triangle.h>
#include <fstream>
#include <iterator>

namespace Chimes
{
	namespace geometry
	{
        template <typename VertexType, typename FaceType>
        requires concept_point_operater<VertexType>&& concept_point_normal<VertexType>&& concept_point_ostream<VertexType>&& concept_point_istream<VertexType>&& concept_point_index<VertexType>
            class PolygonMesh;

        template <typename V, typename F>
        class VertexIterator : public std::iterator<std::random_access_iterator_tag, V>
        {
            friend class PolygonMesh<V, F>;
        public:
            explicit VertexIterator(PolygonMesh<V, F>* mesh, size_t cursor) : mesh_(mesh), cursor_(cursor)
            {

            }
            VertexIterator& operator=(const VertexIterator& v_iter)
            {
                mesh_ = v_iter.mesh_;
                cursor_ = v_iter.cursor_;
                return *this;
            }
            VertexIterator& operator++()
            {
                if (cursor_ >= mesh_->NumberOfVertices())
                {
                    throw std::logic_error("Cannot increment an end iterator");
                }
                cursor_++;
                return *this;
            }
            VertexIterator operator++(int)
            {
                if (cursor_ >= mesh_->NumberOfVertices())
                {
                    throw std::logic_error("Cannot increment an end iterator");
                }
                VertexIterator tmp = *this;
                cursor_++;
                return tmp;
            }
            V& operator*()
            {
                if (cursor_ < 0 or cursor_ >= mesh_->NumberOfVertices())
                {
                    throw std::logic_error("Cannot dereference an invalid iterator");
                }
                return mesh_->vertices_[cursor_];
            }
            V* operator->()
            {
                if (cursor_ < 0 or cursor_ >= mesh_->NumberOfVertices())
                {
                    throw std::logic_error("Cannot dereference an invalid iterator");
                }
                return &mesh_->vertices_[cursor_];
            }
            bool operator==(const VertexIterator& v_iter) const
            {
                return cursor_ == v_iter.cursor_;
            }
            bool operator!=(const VertexIterator& v_iter) const
            {
                return cursor_ != v_iter.cursor_;
            }
        protected:
            PolygonMesh<V, F>* mesh_;
            size_t cursor_;
        };

        template <typename V, typename F>
        class FaceIterator : public std::iterator<std::random_access_iterator_tag, F>
        {
            friend class PolygonMesh<V, F>;
        public:
            explicit FaceIterator(PolygonMesh<V, F>* mesh, size_t cursor) : mesh_(mesh), cursor_(cursor)
            {

            }
            FaceIterator& operator=(const FaceIterator& f_iter)
            {
                mesh_ = f_iter.mesh_;
                cursor_ = f_iter.cursor_;
                return *this;
            }
            FaceIterator& operator++()
            {
                if (cursor_ >= mesh_->NumberOfFaces())
                {
                    throw std::logic_error("Cannot increment an end iterator");
                }
                cursor_++;
                return *this;
            }
            FaceIterator operator++(int)
            {
                if (cursor_ >= mesh_->NumberOfFaces())
                {
                    throw std::logic_error("Cannot increment an end iterator");
                }
                FaceIterator tmp = *this;
                cursor_++;
                return tmp;
            }
            F& operator*()
            {
                if (cursor_ < 0 or cursor_ >= mesh_->NumberOfFaces())
                {
                    throw std::logic_error("Cannot dereference an invalid iterator");
                }
                return mesh_->faces_[cursor_];
            }
            F* operator->()
            {
                if (cursor_ < 0 or cursor_ >= mesh_->NumberOfVertices())
                {
                    throw std::logic_error("Cannot dereference an invalid iterator");
                }
                return &mesh_->faces_[cursor_];
            }
            bool operator==(const FaceIterator& f_iter) const
            {
                return cursor_ == f_iter.cursor_;
            }
            bool operator!=(const FaceIterator& f_iter) const
            {
                return cursor_ != f_iter.cursor_;
            }
        protected:
            PolygonMesh<V, F>* mesh_;
            size_t cursor_;
        };

        //The base class of polygon mesh, which contains a set of vertices and polygons. Both vertices and polygons are contiguous storage in memory.
        template <typename VertexType, typename FaceType>
            requires concept_point_operater<VertexType>&& concept_point_normal<VertexType>&& concept_point_ostream<VertexType>&& concept_point_istream<VertexType>&& concept_point_index<VertexType>
        class PolygonMesh : public GeometryObject
        {
                friend class VertexIterator<VertexType, FaceType>;
                friend class FaceIterator<VertexType, FaceType>;
        private:
            using Real = typename VertexType::R;
        public:
            //Empty initialization.
            PolygonMesh() :vertex_data_(0), vertices_(0), index_data_(0), faces_(0)
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
            const VertexType& Vertex(size_t i) const
            {
                return vertices_[i];
            }
            //Return the number of faces.
            size_t NumberOfFaces() const
            {
                return faces_.size();
            }
            // Return the i_th face.
            const FaceType& Face(size_t i) const
            {
                return faces_[i];
            }
            VertexIterator<VertexType, FaceType> VBegin()
            {
                return VertexIterator<VertexType, FaceType>(this, 0);
            }
            VertexIterator<VertexType, FaceType> VEnd()
            {
                return VertexIterator<VertexType, FaceType>(this, vertices_.size());
            }
            FaceIterator<VertexType, FaceType> FBegin()
            {
                return FaceIterator<VertexType, FaceType>(this, 0);
            }
            FaceIterator<VertexType, FaceType> FEnd()
            {
                return FaceIterator<VertexType, FaceType>(this, faces_.size());
            }
        protected:
            MemoryPtr<Real> vertex_data_;
            std::vector<VertexType> vertices_;
            MemoryPtr<size_t> index_data_;
            std::vector<FaceType> faces_;
        };
        //The common triangle mesh, which contains a set of vertices and triangles. Both vertices and triangles are contiguous storage in memory
        template <typename V>
        class TriangleMesh : public PolygonMesh<V, Triangle<V>>
        {
        private:
            using Base = PolygonMesh<V, Triangle<V>>;
            using Tri = Triangle<V>;
            using Real = typename V::R;
        public:
            //Load from file.
            //The data is saved.
            TriangleMesh(const std::string& file) : Base()
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
                for (size_t i = 0; i < Base::faces_.size(); ++i)
                {
                    out << "f " << Base::faces_[i].Pid(0) + 1 << " " << Base::faces_[i].Pid(1) + 1 << " " << Base::faces_[i].Pid(2) + 1 << std::endl;
                }
                out.close();
                return true;
            }
            //Return the geometry type.
            virtual GeometryType Info() const
            {
                return GeometryType::TRIANGLE_MESH;
            }
        protected:
            //Load mesh from obj file.
            bool load_from_obj(const std::string& file)
            {
                std::stringstream scream_line;
                std::ifstream in(file);
                Base::vertices_.clear();
                Base::faces_.clear();
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
                Base::index_data_ = MemoryPtr<size_t>(index_xyz.size());
                Base::vertices_.reserve(points_xyz.size() / 3);
                for (size_t i = 0; i < points_xyz.size(); i += 3)
                {
                    Base::vertex_data_[i] = points_xyz[i];
                    Base::vertex_data_[i + 1] = points_xyz[i + 1];
                    Base::vertex_data_[i + 2] = points_xyz[i + 2];                    
                    Base::vertices_.emplace_back(V(Base::vertex_data_, i));
                }
                Base::faces_.reserve(index_xyz.size() / 3);
                for (size_t i = 0; i < index_xyz.size(); i += 3)
                {
                    Base::index_data_[i] = index_xyz[i];
                    Base::index_data_[i + 1] = index_xyz[i + 1];
                    Base::index_data_[i + 2] = index_xyz[i + 2];
                    std::vector<V> tri_points(3, 0);
                    tri_points[0].share(Base::vertices_[Base::index_data_[i]]);
                    tri_points[1].share(Base::vertices_[Base::index_data_[i + 1]]);
                    tri_points[2].share(Base::vertices_[Base::index_data_[i + 2]]);
                    Base::faces_.emplace_back(Tri(std::move(tri_points)));
                    Base::faces_.back().SetPid(Base::index_data_, i);
                }                
                return true;
            }
        };
	} // namespace geometry
} // namespace Chimes
