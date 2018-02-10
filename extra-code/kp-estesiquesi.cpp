#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cassert>
using namespace std;

using std::cout;
using std::endl;
using std::vector;

inline size_t next_pow_of_2(size_t n)
{
  --n;  //the following works for 32 bit ints
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  return ++n;
}

struct point
{
  double x, y;

  point() : x(0.), y(0.)
  {}
  point(double u, double v) : x(u), y(v)
  {}
};

struct kd_tree
{
  struct node
  {
    size_t i0, i1, split;

    node() : i0(-1), i1(-1), split(-1)
    {}
    friend bool is_leaf(const node &n) { return n.split==size_t(-1); }
  };
  point min, max;
  vector<node> nodes;

  kd_tree(const vector<point> &points);
  template<typename distance_t>
  double nearest(const point &p,
           const vector<point> &points,
           distance_t &dist,
           double distance = std::numeric_limits<double>::max()) const;
};

struct build_task
{
  size_t first, last, node, dim;
};

inline void expand(point &min, point &max, const point &p)
{
  if (p.x<min.x) min.x = p.x;
  else if (p.x>max.x) max.x = p.x;
  if (p.y<min.y) min.y = p.y;
  else if (p.y>max.y) max.y = p.y;
  return;
}

struct lower_x
{
  const std::vector<point> &points;

  lower_x(const std::vector<point> &p) : points(p)
  {}
  bool operator()(size_t i1, size_t i2) const
  { return points[i1].x<points[i2].x; }
};

struct lower_y
{
  const std::vector<point> &points;

  lower_y(const std::vector<point> &p) : points(p)
  {}
  bool operator()(size_t i1, size_t i2) const
  { return points[i1].y<points[i2].y; }
};

kd_tree::kd_tree(const std::vector<point> &points)
{
  const int stack_size = 32;
  const size_t count = points.size();
  if (count==0)
    return;
  size_t *index = new size_t[count];
  for (size_t i = 0; i<count; ++i)
    index[i] = i;
  const size_t
    m = next_pow_of_2(count),
    node_count = std::min(m - 1, 2*count - m/2 - 1);
  nodes.reserve(node_count);
  nodes.push_back(node());
  build_task tasks[stack_size] = {{/*first*/0, /*last*/count - 1, /*node*/0, /*dim*/0}};
  int current_task = 0;
  point
    min(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()),
    max(-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
  do
  {
    build_task task = tasks[current_task];
    node &n = nodes[task.node];
    if (task.last - task.first<=1)
    {
      n.i0 = index[task.first];
      expand(min, max, points[n.i0]);
      if (task.first!=task.last)
      {
        n.i1 = index[task.last];
        expand(min, max, points[n.i0]);
      }
      assert(is_leaf(n));
      --current_task;
      continue;
    }
    const size_t k = (task.first + task.last)/2;
    if (task.dim==0)
      std::nth_element(index + task.first, index + k, index + task.last + 1, lower_x(points));
    else
      std::nth_element(index + task.first, index + k, index + task.last + 1, lower_y(points));
    const size_t
      i0 = nodes.size(),
      i1 = i0 + 1;
    n.split = index[k];
    n.i0 = i0;
    n.i1 = i1;
    const size_t next_dir = task.dim^1;
    const build_task
      task0 = {task.first, k, i0, next_dir},
      task1 = {k + 1, task.last, i1, next_dir};
    tasks[current_task] = task0;
    nodes.push_back(node());
    ++current_task;
    assert(current_task<stack_size);
    tasks[current_task] = task1;
    nodes.push_back(node());
  } while (current_task!=-1);
  delete [] index;
  assert(nodes.size()==node_count);
  this->min = min;
  this->max = max;
}

struct traverse_task
{
  point c;
  double dist;
  size_t node, dim;
};

inline double bind_to_segment(double x, double x0, double x1)
{
  if (x<x0)
    return x0;
  else if (x>x1)
    return x1;
  return x;
}

inline point bind_to_rect(const point &p, const point &min, const point &max)
{
  return point(bind_to_segment(p.x, min.x, max.x), bind_to_segment(p.y, min.y, max.y));
}

template<typename distance_t>
double kd_tree::nearest(const point &p,
                        const vector<point> &points,
                        distance_t &dist,
                        double distance) const
{
  const int stack_size = 32;
  const point c = bind_to_rect(p, min, max);
  traverse_task tasks[stack_size] = {{c, dist(p, c), /*node*/0, /*dim*/0}};
  int current_task = 0;
  size_t best = -1;
  do
  {
    if (tasks[current_task].dist>distance)
      continue;
    for (;;)
    {
      const traverse_task &task = tasks[current_task];
      const node &n = nodes[task.node];
      if (is_leaf(n))
      {
        assert(n.i0!=size_t(-1));
        const double d = dist(p, points[n.i0]);
        if (d<distance)
        {
          distance = d;
          best = n.i0;
        }
        if (n.i1!=size_t(-1))
        {
          const double d = dist(p, points[n.i1]);
          if (d<distance)
          {
            distance = d;
            best = n.i1;
          }
        }
        break;
      }
      traverse_task near, far;
      point c = task.c;
      bool lower_is_near;
      if (task.dim==0)
      {
        const double split = points[n.split].x;
        c.x = split;
        lower_is_near = p.x<=split;
      }
      else
      {
        const double split = points[n.split].y;
        c.y = split;
        lower_is_near = p.y<=split;
      }
      if (lower_is_near)
      {
        near.node = n.i0;
        far.node = n.i1;
      }
      else
      {
        near.node = n.i1;
        far.node = n.i0;
      }
      const size_t next_dim = task.dim^1;
      near.c = task.c;
      near.dist = 0.;
      near.dim = next_dim;
      far.c = c;
      far.dist = dist(p, far.c);
      far.dim = next_dim;
      tasks[current_task] = far;
      ++current_task;
      assert(current_task<stack_size);
      tasks[current_task] = near;
    }
  } while (--current_task!=-1);
  return distance;
}

template<typename distance_t>
double nearest(const point &p,
         const vector<point> &points,
         distance_t &dist,
         double distance = std::numeric_limits<double>::max())
{
  size_t n = size_t(-1);
  const size_t count = points.size();
  cout << "Este es el que tal \n";
  for (size_t i = 0; i<count; ++i)
  {
    const double d = dist(p, points[i]);
    cout << "Este es el d :: " << d << endl;
    if (d<distance)
    {
      distance = d;
      n = i;
    }
  }
  return distance;
}

inline double randf(double max = 1.) { return max*rand()/RAND_MAX; }
inline double sqr(double a) { return a*a; }

struct squared_distance
{
  double operator()(const point &p, const point &q) const { return sqr(p.x - q.x) + sqr(p.y - q.y); }
};

template<typename distance_t>
size_t baseline_test(const vector<point> &points, distance_t &dist, size_t n = 1, double range = std::numeric_limits<double>::max())
{
  srand(0);
  size_t c = 0;
  for (size_t i = 0; i<n; ++i)
  {
    const point p(randf(), randf());
    c ^= nearest(p, points, dist, range);
  }
  return c;
}

template<typename distance_t>
size_t kdtree_test(const kd_tree &tree, const vector<point> &points, distance_t &dist, size_t n = 1, double range = std::numeric_limits<double>::max())
{
  srand(0);
  size_t c = 0;
  for (size_t i = 0; i<n; ++i)
  {
    const point p(randf(), randf());
    c ^= tree.nearest(p, points, dist, range);
  }
  return c;
}

template<size_t count>
void test(size_t n = 1, double range = std::numeric_limits<double>::max())
{
  vector<point> points(count);
  for (size_t i = 0; i<count; ++i)
    points[i] = point(randf(), randf());
  squared_distance dist;
  const size_t baseline = baseline_test(points, dist, n, range);
  const kd_tree tree(points);
  const size_t kdtree = kdtree_test(tree, points, dist, n, range);
  cout << "test<" << count << ">(" << n << "): " << (baseline==kdtree ? "ok" : "error") << "\n";
  return;
}

typedef vector<point> vPoint;

int main(int argc, const char *argv[])
{
    //-- Declare variables
    int nConjunto1, nConjunto2;
    vPoint conjunto1, conjunto2;

    //-- Read from stdin
    int x, y;
    cin >> nConjunto1;
    conjunto1.reserve(nConjunto1);
    for(int i=0; i<nConjunto1; i++){ 
        cin >> x; cin >> y; 
        conjunto1.push_back(point(x, y)); 
    }

    cin >> nConjunto2;
    conjunto2.reserve(nConjunto2);
    for(int i=0; i<nConjunto2; i++){ 
        cin >> x; cin >> y; 
        conjunto2.push_back(point(x, y)); 
    }

    const kd_tree tree(conjunto1);

    for(int i=0; i<nConjunto2; i++){ 
        squared_distance dist;
        point p(conjunto2[i].x, conjunto2[i].y);
        //assert(tree.nearest(p, conjunto1, dist)==nearest(p, conjunto1, dist));
        double distance = tree.nearest(p, conjunto1, dist);
        cout << distance << " ";
       // assert(tree.nearest(p, conjunto1, dist)==nearest(p, conjunto1, dist));
    }
    cout << endl;


    /*
  squared_distance dist;
  const point p(randf(), randf());
  assert(tree.nearest(p, points, dist)==nearest(p, points, dist));
  cout << "true nearest point is #" << nearest(p, points, dist)
    << "\nkd tree nearest point is #" << tree.nearest(p, points, dist)
    << "\n";

  const size_t iter_count = argc>1 ? size_t(atoi(argv[1])) : 5000;*/


  /*cout << "true nearest point is #" << nearest(p, points, dist)
    << "\nkd tree nearest point is #" << tree.nearest(p, points, dist)
    << "\n";*/

}