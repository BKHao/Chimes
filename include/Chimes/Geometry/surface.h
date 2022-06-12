// 09/05/2022 by BKHao in Chimes.
#pragma once
#include <Chimes/Core/template_concept.h>
#include <Chimes/Core/iterator_base.h>
#include <Chimes/Geometry/geometry_object.h>
#include <Chimes/Geometry/triangle.h>
#include <Chimes/Geometry/segment.h>
#include <fstream>
#include <iterator>
#include <map>

namespace Chimes
{
	namespace geometry
	{
        template <typename VertexType, typename FaceType>
        requires concept_point_operater<VertexType>&& concept_point_normal<VertexType>&& concept_point_ostream<VertexType>&& concept_point_istream<VertexType>&& concept_point_index<VertexType>
            class PolygonMesh;

        template <typename VertexType, typename FaceType>
        class VertexIterator : public std::iterator<std::random_access_iterator_tag, VertexType>
        {
            friend class PolygonMesh<VertexType, FaceType>;
        public:
            explicit VertexIterator(PolygonMesh<VertexType, FaceType>* mesh, size_t cursor) : mesh_(mesh), cursor_(cursor)
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
            VertexType& operator*()
            {
                if (cursor_ < 0 or cursor_ >= mesh_->NumberOfVertices())
                {
                    throw std::logic_error("Cannot dereference an invalid iterator");
                }
                return mesh_->vertices_[cursor_];
            }
            VertexType* operator->()
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
            PolygonMesh<VertexType, FaceType>* mesh_;
            size_t cursor_;
        };

        template <typename VertexType, typename FaceType>
        class FaceIterator : public std::iterator<std::random_access_iterator_tag, FaceType>
        {
            friend class PolygonMesh<VertexType, FaceType>;
        public:
            explicit FaceIterator(PolygonMesh<VertexType, FaceType>* mesh, size_t cursor) : mesh_(mesh), cursor_(cursor)
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
            FaceType& operator*()
            {
                if (cursor_ < 0 or cursor_ >= mesh_->NumberOfFaces())
                {
                    throw std::logic_error("Cannot dereference an invalid iterator");
                }
                return mesh_->faces_[cursor_];
            }
            FaceType* operator->()
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
            PolygonMesh<VertexType, FaceType>* mesh_;
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
        template <typename VertexType>
        class TriangleMesh : public PolygonMesh<VertexType, Triangle<VertexType>>
        {
        private:
            using Base = PolygonMesh<VertexType, Triangle<VertexType>>;
            using Tri = Triangle<VertexType>;
            using Real = typename VertexType::R;
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
                    Base::vertices_.emplace_back(VertexType(Base::vertex_data_, i));
                }
                Base::faces_.reserve(index_xyz.size() / 3);
                for (size_t i = 0; i < index_xyz.size(); i += 3)
                {
                    Base::index_data_[i] = index_xyz[i];
                    Base::index_data_[i + 1] = index_xyz[i + 1];
                    Base::index_data_[i + 2] = index_xyz[i + 2];
                    //std::vector<VertexType> tri_points(3, 0);
                    //tri_points[0].share(Base::vertices_[Base::index_data_[i]]);
                    //tri_points[1].share(Base::vertices_[Base::index_data_[i + 1]]);
                    //tri_points[2].share(Base::vertices_[Base::index_data_[i + 2]]);
                    Base::faces_.emplace_back(Tri(Base::index_data_, i));
                    //Base::faces_.back().SetPid(Base::index_data_, i);
                }                
                return true;
            }
        };

        template <typename VertexType>
        class ManifoldTriangleMesh : public TriangleMesh<VertexType>
        {
        private:
            using Base = TriangleMesh<VertexType>;
            using Tri = Triangle<VertexType>;
            using Real = typename VertexType::R;
        public:
            class MTMEdge :public Segment<VertexType>
            {
            public:
                MTMEdge() :Segment<VertexType>()
                {
                    start_vertex_ = -1;
                    end_vertex_ = -1;
                    opposite_vertex_ = -1;
                    prv_edge_ = -1;
                    next_edge_ = -1;
                    reverse_edge_ = -1;
                    face_ = -1;
                }
                MTMEdge(size_t i0, size_t i1) : Segment<VertexType>(i0, i1)
                {
                    start_vertex_ = i0;
                    end_vertex_ = i1;
                    opposite_vertex_ = -1;
                    prv_edge_ = -1;
                    next_edge_ = -1;
                    reverse_edge_ = -1;
                    face_ = -1;
                }
            public:
                int start_vertex_;
                int end_vertex_;
                int opposite_vertex_;
                int prv_edge_;
                int next_edge_;
                int reverse_edge_;
                int face_;
            };
        public:
            //Load from file.
            //The data is saved.
            ManifoldTriangleMesh(const std::string& file) : Base(file)
            {
                OrganizeHalfEdge();
            }
        private:
            void OrganizeHalfEdge()
            {
                halfedges_.resize(0);
                halfedges_.reserve(Base::NumberOfFaces() * 3);
                std::map<std::pair<size_t, size_t>, size_t> from_ipair_to_location;
                std::vector<size_t> eid_in_f(3, -1);
                for (size_t i = 0; i < Base::faces_.size(); ++i)
                {
                    eid_in_f.clear();
                    eid_in_f.resize(3, -1);
                    for (size_t j = 0; j < 3; ++j)
                    {
                        size_t next = (j + 1) % 3;
                        size_t prv = (j + 2) % 3;
                        size_t start_vertex = Base::faces_[i].Pid(prv);
                        size_t end_vertex = Base::faces_[i].Pid(j);
                        std::map<std::pair<size_t, size_t>, size_t>::const_iterator it = from_ipair_to_location.find(std::make_pair(start_vertex, end_vertex));
                        if (it != from_ipair_to_location.end())
                        {
                            size_t loc_in_edges = it->second;
                            if (halfedges_[loc_in_edges].opposite_vertex_ != -1)
                            {
                                throw std::runtime_error("repeated edges.");
                            }
                            eid_in_f[j] = loc_in_edges;
                            halfedges_[loc_in_edges].opposite_vertex_ = Base::faces_[i].Pid(next);
                            halfedges_[loc_in_edges].face_ = i;
                        }
                        else
                        {
                            halfedges_.emplace_back(MTMEdge(start_vertex, end_vertex));
                            halfedges_.back().face_ = i;
                            halfedges_.back().opposite_vertex_ = Base::faces_[i].Pid(next);
                            halfedges_.back().reverse_edge_ = halfedges_.size();
                            from_ipair_to_location[std::make_pair(start_vertex, end_vertex)] = eid_in_f[j] = halfedges_.size() - 1;

                            halfedges_.emplace_back(MTMEdge(end_vertex, start_vertex));
                            halfedges_.back().reverse_edge_ = halfedges_.size() - 2;
                            from_ipair_to_location[std::make_pair(end_vertex, start_vertex)] = halfedges_.size() - 1;
                        }
                    }
                    for (size_t j = 0; j < 3; ++j)
                    {
                        halfedges_[eid_in_f[j]].prv_edge_ = eid_in_f[(j + 2) % 3];
                        halfedges_[eid_in_f[j]].next_edge_ = eid_in_f[(j + 1) % 3];
                    }
                }
                neighbor_edge_of_vertex_.clear();
                neighbor_edge_of_vertex_.resize(Base::vertices_.size(), -1);
                neighbor_edge_of_face_.clear();
                neighbor_edge_of_face_.resize(Base::faces_.size(), -1);
                for (auto it = halfedges_.begin(); it != halfedges_.end(); ++it)
                {
                    if (it->face_ == -1)
                        continue;
                    if (neighbor_edge_of_vertex_[it->start_vertex_] == -1 || halfedges_[it->reverse_edge_].face_ == -1)
                    {
                        neighbor_edge_of_vertex_[it->start_vertex_] = it - halfedges_.begin();
                    }
                    if (Base::faces_[it->face_].Pid(0) == it->opposite_vertex_)
                    {
                        neighbor_edge_of_face_[it->face_] = it - halfedges_.begin();
                    }
                }
            }
        private:
            std::vector<MTMEdge> halfedges_;
            std::vector<int> neighbor_edge_of_vertex_;
            std::vector<int> neighbor_edge_of_face_;
        };
	} // namespace geometry
} // namespace Chimes
