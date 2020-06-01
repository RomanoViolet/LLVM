#ifndef TREE_HPP_
#define TREE_HPP_

class Tree final
{
public:
  Tree( ) = default;
  void insert( int parent, Data data );
  bool exists( Data data );
  void delete ( Data data ) = delete;
  void get( );
};

#endif  // !TREE_HPP_
