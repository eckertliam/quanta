# Quanta
A keyword based language for clean elegant code.

## Example
This example can be found in the `examples` directory.
```quanta
# define a struct
struct Point
    x: i32
    y: i32
end

impl Init for Point
    fn init(x: i32, y: i32) -> Point
        Point { x, y }
    end
end

impl Add for Point
    fn add(self, other: Point) -> Point
        Point(self.x + other.x, self.y + other.y)
    end
end

# define a function
fn main()
    const p1 = Point.init(1, 2);
    const p2 = Point.init(3, 4);
    const p3 = p1 + p2;
    println(p3)
end
```
Quanta aims to be clear and concise. It tries to stay close to other languages such as Julia, Rust, and Python.
It is statically typed and has a strong type inference system.
