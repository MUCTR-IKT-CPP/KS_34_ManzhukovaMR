#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <string>
#include <vector>

namespace
{
    constexpr double k_pi = 3.14159265358979323846;
}

/**
 * @brief Interface for shapes that support uniform scaling.
 */
class IScalable
{
public:
    virtual ~IScalable() = default;

    /**
     * @brief Scales the shape by the given factor.
     * @param factor Positive scaling multiplier.
     */
    virtual void scale(double factor) = 0;
};

/**
 * @brief Abstract base class for all geometric shapes.
 */
class Shape
{
public:
    virtual ~Shape() = default;
    virtual double area() 
    ;
    virtual double perimeter() const = 0;
    virtual void printInfo() const = 0;
};

/**
 * @brief Circle defined by its radius.
 */
class Circle final : public Shape, public IScalable
{
private:
    double _radius;

public:
    /**
     * @brief Constructs a circle with the given radius.
     * @param radius Positive radius value.
     * @throws std::invalid_argument If radius is not positive.
     */
    explicit Circle(double radius) : _radius{radius}
    {
        if (radius <= 0.0)
        {
            throw std::invalid_argument{"Circle radius must be positive"};
        }
    }

    double area() const override
    {
        return k_pi * _radius * _radius;
    }

    double perimeter() const override
    {
        return 2.0 * k_pi * _radius;
    }

    void printInfo() const override
    {
        std::cout << "Circle: radius = " << _radius
                  << ", area = " << area()
                  << ", perimeter = " << perimeter() << '\n';
    }

    /**
     * @brief Scales the circle by multiplying its radius.
     * @param factor Positive scaling multiplier.
     * @throws std::invalid_argument If factor is not positive.
     */
    void scale(double factor) override
    {
        if (factor <= 0.0)
        {
            throw std::invalid_argument{"Scale factor must be positive"};
        }
        _radius *= factor;
    }
};

/**
 * @brief Axis-aligned rectangle defined by width and height.
 */
class Rectangle : public Shape, public IScalable
{
private:
    double _width;
    double _height;

public:
    /**
     * @brief Constructs a rectangle with the given dimensions.
     * @param width Positive width value.
     * @param height Positive height value.
     * @throws std::invalid_argument If any dimension is not positive.
     */
    Rectangle(double width, double height)
        : _width{width}, _height{height}
    {
        if (width <= 0.0 || height <= 0.0)
        {
            throw std::invalid_argument{"Rectangle sides must be positive"};
        }
    }

    double area() const override
    {
        return _width * _height;
    }

    double perimeter() const override
    {
        return 2.0 * (_width + _height);
    }

    void printInfo() const override
    {
        std::cout << "Rectangle: width = " << _width
                  << ", height = " << _height
                  << ", area = " << area()
                  << ", perimeter = " << perimeter() << '\n';
    }

    /**
     * @brief Scales the rectangle by multiplying both dimensions.
     * @param factor Positive scaling multiplier.
     * @throws std::invalid_argument If factor is not positive.
     */
    void scale(double factor) override
    {
        if (factor <= 0.0)
        {
            throw std::invalid_argument{"Scale factor must be positive"};
        }
        _width *= factor;
        _height *= factor;
    }
};

/**
 * @brief Triangle defined by three side lengths.
 */
class Triangle final : public Shape
{
private:
    double _side_a;
    double _side_b;
    double _side_c;

public:
    /**
     * @brief Constructs a triangle with the given sides.
     * @param sideA Length of the first side.
     * @param sideB Length of the second side.
     * @param sideC Length of the third side.
     * @throws std::invalid_argument If any side is not positive
     *         or the triangle inequality is violated.
     */
    Triangle(double sideA, double sideB, double sideC)
        : _side_a{sideA}, _side_b{sideB}, _side_c{sideC}
    {
        if (sideA <= 0.0 || sideB <= 0.0 || sideC <= 0.0)
        {
            throw std::invalid_argument{"Triangle sides must be positive"};
        }
        if (sideA + sideB <= sideC || sideA + sideC <= sideB || sideB + sideC <= sideA)
        {
            throw std::invalid_argument{"Triangle inequality violated"};
        }
    }

    double area() const override
    {
        const double semiPerimeter{perimeter() / 2.0};
        return std::sqrt(semiPerimeter * (semiPerimeter - _side_a) * (semiPerimeter - _side_b) * (semiPerimeter - _side_c));
    }

    double perimeter() const override
    {
        return _side_a + _side_b + _side_c;
    }

    void printInfo() const override
    {
        std::cout << "Triangle: sides = " << _side_a << ", " << _side_b
                  << ", " << _side_c
                  << ", area = " << area()
                  << ", perimeter = " << perimeter() << '\n';
    }
};

/**
 * @brief Aggregates shapes and exposes their combined area and perimeter.
 *
 * Holds non-owning pointers; the lifetime of the shapes is managed externally.
 */
class ShapeGroup final : public Shape
{
private:
    std::vector<Shape *> _shapes;

public:
    /**
     * @brief Adds a shape to the group. Null pointers are ignored.
     * @param shape Non-owning pointer to the shape.
     */
    void addShape(Shape *shape)
    {
        if (shape != nullptr)
        {
            _shapes.push_back(shape);
        }
    }

    double totalArea() const
    {
        double total{0.0};
        for (const Shape *shape : _shapes)
        {
            total += shape->area();
        }
        return total;
    }
    double totalPerimeter() const
    {
        double total{0.0};
        for (const Shape *shape : _shapes)
        {
            total += shape->perimeter();
        }
        return total;
    }
    double area() const override
    {
        return totalArea();
    }
    double perimeter() const override
    {
        return totalPerimeter();
    }

    void printInfo() const override
    {
        std::cout << "ShapeGroup: contains " << _shapes.size()
                  << " shapes, total area = " << totalArea()
                  << ", total perimeter = " << totalPerimeter() << '\n';
    }
};

/**
 * @brief Attempts to scale a shape through the IScalable interface.
 * @param shape Reference to the shape to scale.
 * @param factor Positive scaling multiplier.
 */
void tryScale(Shape &shape, double factor)
{
    if (auto *scalable = dynamic_cast<IScalable *>(&shape))
    {
        scalable->scale(factor);
    }
}

int main()
{
    Circle circle{5.0};
    Rectangle rectangle{4.0, 6.0};
    Triangle triangle{3.0, 4.0, 5.0};

    std::vector<std::unique_ptr<Shape>> shapes{};
    shapes.push_back(std::make_unique<Circle>(circle));
    shapes.push_back(std::make_unique<Rectangle>(rectangle));
    shapes.push_back(std::make_unique<Triangle>(triangle));

    std::cout << "=== Initial shapes ===\n";
    for (const auto &shape : shapes)
    {
        shape->printInfo();
    }

    ShapeGroup group{};
    for (const auto &shape : shapes)
    {
        group.addShape(shape.get());
    }

    std::cout << "\n=== Shape group ===\n";
    group.printInfo();

    constexpr double factor{2.0};
    std::cout << "\n=== After scaling circles and rectangles by "
              << factor << " ===\n";

    for (const auto &shape : shapes)
    {
        tryScale(*shape, factor);
    }

    for (const auto &shape : shapes)
    {
        shape->printInfo();
    }

    std::cout << "\n=== Updated shape group ===\n";
    group.printInfo();

    return 0;
}