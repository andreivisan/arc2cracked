# Kouka

## About

Kouka is high-performance, enterprise-grade Java build tool written in C. It leverages the usage of AI Agents to create and run any
type of Java project.

## Features

### 1. Project Scaffolding & Initialization

Create project “scaffolds” for various Java project types, each with built‑in AI recommendations and a standardized layout.

#### Supported Project Types:

- **Simple JAR Library**: A basic structure for creating a Java archive.
- **Spring Project**: A standard enterprise project using Spring.
- **Spring Boot Project**: A self-contained Spring Boot project.
- **Practice/Starter Project**: A lightweight folder structure for practicing Java.

#### Key Files in Every Scaffold:

- **dependencies.json**:
    - Lists all required libraries (with versions).
    - May include metadata for transitive dependencies.
- **commands.json**:
    - Defines custom command combinations (much like the “scripts” section in package.json).
    - Supports common operations like build, test, run, and even custom AI‑enhanced workflows.

#### AI Integration:

- **AI‑Driven Best Practices**:
    - When scaffolding a project, the AI engine (via your local model) analyzes best practices from similar, 
    proven projects and suggests optimal folder structures, dependency sets, and even initial custom commands.
- **Interactive Mode**:
    - Run in an interactive mode where the AI asks questions to tailor the scaffold to your needs.
    


