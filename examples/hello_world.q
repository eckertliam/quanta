struct Point
    x: i32
    y: i32
end

impl Init for Point
    fn init(x: i32, y: i32) -> Self
    begin
        return Point { x, y };
    end
end

impl Add for Point
    fn add(self, other: Point) -> Self
    begin
        return Point(self.x + other.x, self.y + other.y);
    end
end



fn main() -> void
begin
    const p1 = Point(1, 2);
    const p2 = Point(3, 4);
    # types can be inferred as above or explicitly declared as below
    const p3: Point = p1 + p2;
    println("Hello, World!");
end