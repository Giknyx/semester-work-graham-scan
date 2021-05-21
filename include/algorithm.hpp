#pragma once

#include <stack>
#include <vector>

// Заголовочный файл

namespace itis {

  struct Point {
   public:
    int x{0};
    int y{0};

    Point() = default;

    Point(int x, int y) : x{x}, y{y} {};
  }; // Point struct

  /**
   * To find second element from the top of the stack
   * @param S - stack
   * @return Point element second from the top of S
   */
  Point nextToTop(std::stack<Point> &S);

  /**
   * Function to calculate the orientation of the turn that three points are making (ccw for counterclockwise)
   * @param p0
   * @param p1
   * @param p2
   * @return 0 if all points are collinear, 1 if they are making clockwise turn, 2 if counterclockwise
   */
  int ccw(Point p0, Point p1, Point p2);

  /**
   * Swap two points
   * @param p1
   * @param p2
   */
  void swap(Point &p1, Point &p2);

  /**
   * Distance between two points
   * @param p1
   * @param p2
   * @return distance
   */
  int dist(Point p1, Point p2);

  /**
   * Compare function for qsort_s. We need to have access to p0 (leftmost lowest point)
   * to compare points p1 and p2 by their angle with p0.
   * @param vp0 - additional parameter for passing p0 to the compare function
   * @param vp1 - first point to compare
   * @param vp2 - second point to compare
   * @return -1 if point p1 has smaller angle with p0 than p2
   *          1 if point p2 has smaller angle with p0 than p1
   *          if they are collinear then returns -1 if p1 is the farthest from p0, 1 if p2 is the farthest from p0
   */
  int compare(void *vp0, const void *vp1, const void *vp2);

  /**
   * Struct for the algorithm.
   */
  struct Graham {
   public:

    /**
     * Adds point with coordinates x, y to the field
     * @param x
     * @param y
     */
    void add_point(int x, int y);

    /**
     * Calculates the convex hull and puts the answer to the stack
     */
    void convex_hull();

    /**
     * Calculates the convex hull, puts the answer to the stack and prints it
     */
    void convex_hull_print();

    /**
     * Clears the stack
     */
    void clear_stack();

    /**
     * Prints the stack
     */
    void print_stack();

   private:
    int size_{0};
    Point p0; // Leftmost lowest point
    std::vector<Point> points;
    std::stack<Point> S;

    /**
     * Finds leftmost lowest point from the field
     * @return leftmost lowest point
     */
    int find_bottom_left();
  }; // Graham struct
}  // namespace itis