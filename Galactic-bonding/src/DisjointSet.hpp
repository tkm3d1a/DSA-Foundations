#include <vector>

class DisjointSet{
private:
  std::vector<int> pset;

public:

  DisjointSet(int MAX_STARS) {
    pset.assign(MAX_STARS,0);
  }

  void initSet(int N){
    for(size_t i = 0; i < N ; i++) {
      pset[i] = i;
    }
  }

  int findSet(int i) {
    if(pset[i] == i){
      return i;
    } else {
      return (pset[i] = findSet(pset[i]));
    }
  }

  bool isSameSet(int i, int j) {
    return findSet(i) == findSet(j);
  }

  void unionSet(int i, int j) {
    pset[findSet(i)] = findSet(j);
  }

  int countSets(int N) {
    int count = 0;

    for(size_t i = 0; i < N; i++) {
      if(pset[i] == i){
        count++;
      }
    }

    return count;
  }
};