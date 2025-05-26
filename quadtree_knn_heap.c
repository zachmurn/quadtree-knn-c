#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MAX_POINTS 4  // Max points per QuadTree node before subdivision

// Structure to represent a 2D point
typedef struct Point {
    double x, y;
} Point;

// Structure to represent a rectangular boundary
typedef struct Rect {
    double x, y;     // Center of the rectangle
    double hw, hh;   // Half-width and half-height
} Rect;

// Structure for a QuadTree node
typedef struct QuadTree {
    Rect boundary;
    Point* points[MAX_POINTS];
    int count;
    struct QuadTree* NW;
    struct QuadTree* NE;
    struct QuadTree* SW;
    struct QuadTree* SE;
} QuadTree;

// Structure for a heap node
typedef struct HeapNode {
    Point* point;
    double distance;
} HeapNode;

// Structure for a max-heap
typedef struct MaxHeap {
    HeapNode* nodes;
    int size;
    int capacity;
} MaxHeap;

// Function to create a new point
Point* create_point(double x, double y) {
    Point* p = (Point*)malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

// Function to create a new rectangle
Rect create_rect(double x, double y, double hw, double hh) {
    Rect r = { x, y, hw, hh };
    return r;
}

// Function to check if a point is within a rectangle
int rect_contains(Rect* r, Point* p) {
    return (p->x >= r->x - r->hw && p->x <= r->x + r->hw &&
            p->y >= r->y - r->hh && p->y <= r->y + r->hh);
}

// Function to check if two rectangles intersect
int rect_intersects(Rect* a, Rect* b) {
    return !(a->x + a->hw < b->x - b->hw || a->x - a->hw > b->x + b->hw ||
             a->y + a->hh < b->y - b->hh || a->y - a->hh > b->y + b->hh);
}

// Function to create a new QuadTree node
QuadTree* create_quadtree(Rect boundary) {
    QuadTree* qt = (QuadTree*)malloc(sizeof(QuadTree));
    qt->boundary = boundary;
    qt->count = 0;
    qt->NW = qt->NE = qt->SW = qt->SE = NULL;
    for (int i = 0; i < MAX_POINTS; i++) {
        qt->points[i] = NULL;
    }
    return qt;
}

// Function to subdivide a QuadTree node
void subdivide(QuadTree* qt) {
    double x = qt->boundary.x;
    double y = qt->boundary.y;
    double hw = qt->boundary.hw / 2;
    double hh = qt->boundary.hh / 2;

    qt->NW = create_quadtree(create_rect(x - hw, y - hh, hw, hh));
    qt->NE = create_quadtree(create_rect(x + hw, y - hh, hw, hh));
    qt->SW = create_quadtree(create_rect(x - hw, y + hh, hw, hh));
    qt->SE = create_quadtree(create_rect(x + hw, y + hh, hw, hh));
}

// Function to insert a point into the QuadTree
int insert(QuadTree* qt, Point* p) {
    if (!rect_contains(&qt->boundary, p)) {
        return 0; // Point not in boundary
    }

    if (qt->count < MAX_POINTS) {
        qt->points[qt->count++] = p;
        return 1;
    }

    if (qt->NW == NULL) {
        subdivide(qt);
    }

    if (insert(qt->NW, p)) return 1;
    if (insert(qt->NE, p)) return 1;
    if (insert(qt->SW, p)) return 1;
    if (insert(qt->SE, p)) return 1;

    return 0; // Should not reach here
}

// Function to calculate Euclidean distance between two points
double distance(Point* a, Point* b) {
    double dx = a->x - b->x;
    double dy = a->y - b->y;
    return sqrt(dx * dx + dy * dy);
}

// Function to create a max-heap
MaxHeap* create_maxheap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->nodes = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Function to swap two heap nodes
void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify up
void heapify_up(MaxHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->nodes[index].distance > heap->nodes[parent].distance) {
            swap(&heap->nodes[index], &heap->nodes[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

// Function to heapify down
void heapify_down(MaxHeap* heap, int index) {
    int left, right, largest;
    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index;

        if (left < heap->size && heap->nodes[left].distance > heap->nodes[largest].distance) {
            largest = left;
        }
        if (right < heap->size && heap->nodes[right].distance > heap->nodes[largest].distance) {
            largest = right;
        }
        if (largest != index) {
            swap(&heap->nodes[index], &heap->nodes[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

// Function to insert into the max-heap
void heap_insert(MaxHeap* heap, Point* p, double dist) {
    if (heap->size < heap->capacity) {
        heap->nodes[heap->size].point = p;
        heap->nodes[heap->size].distance = dist;
        heapify_up(heap, heap->size);
        heap->size++;
    } else if (dist < heap->nodes[0].distance) {
        heap->nodes[0].point = p;
        heap->nodes[0].distance = dist;
        heapify_down(heap, 0);
    }
}

// Recursive function to perform KNN search
void knn_search(QuadTree* qt, Point* target, MaxHeap* heap) {
    if (qt == NULL) return;

    for (int i = 0; i < qt->count; i++) {
        double dist = distance(target, qt->points[i]);
        heap_insert(heap, qt->points[i], dist);
    }

    if (qt->NW == NULL) return;

    knn_search(qt->NW, target, heap);
    knn_search(qt->NE, target, heap);
    knn_search(qt->SW, target, heap);
    knn_search(qt->SE, target, heap);
}

// Function to print the K nearest neighbors
void print_knn(MaxHeap* heap) {
    printf("K Nearest Neighbors:\n");
    for (int i = 0; i < heap->size; i++) {
        printf("Point (%.2f, %.2f) - Distance: %.2f\n",
               heap->nodes[i].point->x, heap->nodes[i].point->y, heap->nodes[i].distance);
    }
}

// Main function to demonstrate the QuadTree KNN
int main() {
    // Define the boundary of the QuadTree
    Rect boundary = create_rect(0.0, 0.0, 100.0, 100.0);
    QuadTree* qt = create_quadtree(boundary);

    // Insert random points into the QuadTree
    for (int i = 0; i < 1000; i++) {
        double x = ((double)rand() / RAND_MAX) * 200.0 - 100.0;
        double y = ((double)rand() / RAND_MAX) * 200.0 - 100.0;
        Point* p = create_point(x, y);
        insert(qt, p);
    }

    // Define the target point
    Point target = { 10.0, 10.0 };

    // Define K
    int K = 5;

    // Create a max-heap for KNN
    MaxHeap* heap = create_maxheap(K);

    // Perform KNN search
    knn_search(qt, &target, heap);

    // Print the K nearest neighbors
    print_knn(heap);

    // Free allocated memory (not shown here for brevity)

    return 0;
}
