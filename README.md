# Classify

## Introduction

This is a simple implementation of a decision tree in C++. I wanted to get a sense for the learning process, so I developed this decision tree implementation which also runs and renders pretty visuals! The main idea is that as it learns to classify between two groups we can observe the learning process.

<img src="https://raw.githubusercontent.com/walkerjameschris/classify/refs/heads/main/img/learn.png" height="250" />

## Contents

- `data/`: Contains the CSV to learn
- `img/`: Screenshot of learning process
- `src/`:
    - `data.hpp`: A custom CSV parser and data loader
    - `simulation.hpp`: The simulation driver
- `CMakeLists.txt`: Make instructions
- `main.cpp`: Entry point
