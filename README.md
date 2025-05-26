# 📍 Quad Tree KNN Search in C

![Yale University Shield](https://en.wikipedia.org/wiki/Coat_of_arms_of_Yale_University#/media/File:Yale_University_Shield_1.svg)




> **Intro to Data Mining — Yale University**  
> Honor project for Vladimir Rokhlin

This is an efficient C implementation of **K-Nearest Neighbors (KNN) search using a recursive Quad Tree**, designed for fast 2D spatial queries.

---

## 🧠 What is KNN Search?

**K-Nearest Neighbors (KNN)** is a fundamental algorithm in data mining and machine learning.

Given a **query point**, it finds the **K closest points** in a dataset.

For example:
> “What are the 3 closest restaurants to my location?”

Naive search checks every point (O(n)).  
This implementation uses **spatial indexing with a Quad Tree** to make it fast.

---

## 🌲 What is a Quad Tree?

A **Quad Tree** is a tree structure that recursively divides 2D space into **four quadrants**. It allows fast lookup by eliminating huge regions of space that cannot contain relevant points.

**Visualization of Quad Tree Subdivision**:

![Quad Tree GIF](https://upload.wikimedia.org/wikipedia/commons/8/8c/Quad_tree_animation.gif)  
<sub>[Source: Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Quad_tree_animation.gif)</sub>

---

## ✨ How This Works

- You insert a set of 2D points into a recursive Quad Tree.
- You query the tree for the **K nearest neighbors** to a target point.
- A **max-heap** is used to efficiently track the current top K closest points.

**KNN Search in Action**:

![KNN GIF](https://upload.wikimedia.org/wikipedia/commons/e/e7/KnnClassification.gif)  
<sub>Note: This is KNN classification, but similar principles apply for search.</sub>

---

## 🧑‍🏫 Course Context

This project was created as part of **Intro to Data Mining** at **Yale University**, exploring:

- Spatial indexing structures
- Geometric algorithms
- Efficient search in 2D space

---

## 🙏 In Honor of Vladimir Rokhlin

![Vladimir Rokhlin](https://cs.yale.edu/sites/default/files/styles/person_thumbnail/public/pictures/picture-100-1437507436.jpg)

> **Vladimir Rokhlin** is a Yale professor and renowned mathematician/computer scientist.  
> He co-invented the **Fast Multipole Method (FMM)** — ranked among the **Top 10 Algorithms of the 20th Century**.

FMM revolutionized how we compute long-range interactions in large systems. This project pays tribute to his influence on efficient computation and spatial methods.

---

## 🚀 Features

- ✅ Recursive Quad Tree
- ✅ Dynamic insertion of points
- ✅ Efficient max-heap-based KNN search
- ✅ Written in **pure C** — portable and lightweight
- ✅ Educational and easy to follow

---

## 🛠️ How to Compile & Run

```bash
gcc quadtree_knn_heap.c -o knn
./knn
