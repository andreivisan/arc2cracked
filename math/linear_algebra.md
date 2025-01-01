# Linear Algebra

## Vectors

### Definition and Notation

- A vector is an ordered collection of numbers (called components or elements), which we often represent in a column form.

    $\mathbf{v} = \begin{pmatrix} v_1 \\ v_2 \\ \vdots \\ v_n \end{pmatrix}$

- In finance, you might have a vector of asset returns, e.g., $\mathbf{r} = (r_1, r_2, \ldots, r_n)^\top$

### Basic Operations

1. Vector Addition

    $\mathbf{u} + \mathbf{v} = \begin{pmatrix} 
    u_1 + v_1 \\ 
    u_2 + v_2 \\ 
    \vdots \\ 
    u_n + v_n 
    \end{pmatrix}$

2. Scalar Multiplication

    $c \mathbf{v} = \begin{pmatrix} 
    cv_1 \\ 
    cv_2 \\ 
    \vdots \\ 
    cv_n 
    \end{pmatrix}$

where $c$ is a scalar.

3. Dot Product (inner product)

    $\mathbf{u} \cdot \mathbf{v} = u_1 v_1 + u_2 v_2 + \cdots + u_n v_n$

4. Norm (Length) of a Vector

- The most common is the Euclidean norm:

    $\|\mathbf{v}\| = \sqrt{\mathbf{v} \cdot \mathbf{v}} = \sqrt{v_1^2 + v_2^2 + \cdots + v_n^2}$

- In finance, norms and distances can be important for measuring errors or distances between return profiles, but more commonly used is the notion of variance and covariance (see below).

