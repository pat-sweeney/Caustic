//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************

mod Math
{
    const c_Epsilon : f32 = 0.0000001;
    fn IsZero(x : f32) -> bool { if x >= -c_Epsilon && x <= c_Epsilon { true } else { false } }
    
    use std::ops;
    
    //**********************************************************************
    // Structure: Vector2
    // Defines a simple vector with 2 components
    //
    // Members:
    // x : f32 - x component
    // y : f32 - y component
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.rs}}
    //**********************************************************************
    #[derive(Debug)]
    struct Vector2
    {
        x : f32,
        y : f32
    }

    impl Vector2
    {
        //**********************************************************************
        // Constructor: Vector2
        // Default contructor
        //**********************************************************************
        fn default() -> Self
        {
            Self { x: 0.0, y : 0.0 }
        }

        //**********************************************************************
        // Constructor: Vector2
        // Contructor for a 2D vector
        //
        // Parameters:
        // _x - X coordinate
        // _y - Y coordinate
        //**********************************************************************
        fn new(_x : f32, _y : f32) -> Self
        {
            Self {  x : _x, y : _y }
        }
 
        //**********************************************************************
        // Method: IsEq
        // Returns whether two points are equivalent (within some tolerance)
        //
        // Parameters:
        // p - Point to compare against
        //
        // Returns:
        // True if points are equivalent. False otherwise.
        //**********************************************************************
        fn IsEq(&self, p : &Self) -> bool
        {
            if IsZero(p.x - self.x) && IsZero(p.y - self.y) { true } else { false }
        }
    
        //**********************************************************************
        // Method: Length
        // Returns the L2 norm for the vector
        //
        // Returns:
        // Length of vector
        //**********************************************************************
        fn Length(&self) -> f32
        {
            (self.x * self.x + self.y * self.y).sqrt()
        }

        //**********************************************************************
        // Method: Normalize
        // Normalizes the vector
        //
        // Returns:
        // Normalized vector
        //**********************************************************************
        fn Normalize(&self) -> Vector2
        {
            let len : f32 = self.Length();
            if !IsZero(len)
            {
                self.x /= len;
                self.y /= len;
            }
            return Vector2::new(self.x, self.y);
        }
        
        //**********************************************************************
        // Define swizzle operators
        //**********************************************************************
        fn xy(&self) -> Vector2
        {
            Self { x : self.x, y : self.y }
        }
        
        fn yx(&self) -> Vector2
        {
            Self { x : self.y, y : self.x }
        }
        
        fn xx(&self) -> Vector2
        {
            Self { x : self.x, y : self.x }
        }
        
        fn yy(&self) -> Vector2
        {
            Self { x : self.y, y : self.y }
        }
    }

    impl ops::Add for Vector2
    {
        type Output = Vector2;
        fn add(self, _rhs : Vector2) -> Vector2
        {
            return Vector2::new(self.x + _rhs.x, self.y + _rhs.y);
        }
    }
    
    impl ops::Sub for Vector2
    {
        type Output = Vector2;
        fn sub(self, _rhs : Vector2) -> Vector2
        {
            return Vector2::new(self.x - _rhs.x, self.y - _rhs.y);
        }
    }
    
    impl ops::Neg for Vector2
    {
        type Output = Vector2;
        fn neg(self) -> Vector2
        {
            return Vector2::new(-self.x, -self.y);
        }
    }
    
    impl ops::Mul<f32> for Vector2
    {
        type Output = Vector2;
        fn mul(self, f : f32) -> Vector2
        {
            return Vector2::new(self.x * f, self.y * f);
        }
    }

    impl ops::Mul<Vector2> for f32
    {
        type Output = Vector2;
        fn mul(self, v : Vector2) -> Vector2
        {
            return Vector2::new(self * v.x, self * v.y);
        }
    }

    impl PartialEq for Vector2
    {
        fn eq(&self, _rhs : &Self) -> bool
        {
            self.IsEq(_rhs)
        }
        
        fn ne(&self, _rhs : &Self) -> bool
        {
            !self.IsEq(_rhs)
        }
    }

    //**********************************************************************
    // Structure: Vector3
    // Defines a simple vector with 3 components
    //
    // Members:
    // x : f32 - x component
    // y : f32 - y component
    // z : f32 - z component
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.rs}}
    //**********************************************************************
    #[derive(Debug)]
    struct Vector3
    {
         x : f32,
         y : f32,
         z : f32
    }

    impl Vector3
    {
        //**********************************************************************
        // Constructor: Vector3
        // Default constructor
        //**********************************************************************
        fn default() -> Self
        {
            Self { x: 0.0, y : 0.0, z : 0.0 }
        }
 
        //**********************************************************************
        // Constructor: Vector3
        // Constructor
        //
        // Parameters:
        // _x - X coordinate
        // _y - Y coordinate
        // _z - Z coordinate
        //**********************************************************************
        fn new(_x : f32, _y : f32, _z : f32) -> Self
        {
            Self { x : _x, y : _y, z : _z }
        }
 
        //**********************************************************************
        // Method: Sign
        // Returns -1 or +1 depending on direction vector is pointing
        //**********************************************************************
        fn Sign(&self) -> i32
        {
            let s0 : i32 = if self.x < 0.0 { -1 } else { 1 };
            let s1 : i32 = if self.y < 0.0 { -1 } else { 1 };
            let s2 : i32 = if self.z < 0.0 { -1 } else { 1 };
            s0 * s1 * s2
        }
 
         //**********************************************************************
         // Method: IsEq
         // Returns whether two points are equivalent (within some tolerance)
         //
         // Parameters:
         // p - point to test against
         //
         // Returns:
         // True if points are considered equivalent. False otherwise.
         //**********************************************************************
         fn IsEq(&self, p : Vector3) -> bool
         {
             if IsZero(p.x - self.x) && IsZero(p.y - self.y) && IsZero(p.z - self.z) { true } else { false }
         }
 
         //**********************************************************************
         // Method: Cross
         // Returns the cross product between two vectors
         //
         // Parameters:
         // v - vector to compute cross product against
         //
         // Returns:
         // The cross product
         //**********************************************************************
         fn Cross(&self, v : Vector3) -> Vector3
         {
             return Vector3::new(self.y * v.z - self.z * v.y,
                     self.z * v.x - self.x * v.z,
                     self.x * v.y - self.y * v.x);
         }
 
         //**********************************************************************
         // Method: Dot
         // Returns the scalar product between two vectors
         //
         // Parameters:
         // v - vector to compute dot product against
         //
         // Returns:
         // The scalar product
         //**********************************************************************
         fn Dot(&self, v : Vector3) -> f32
         {
             self.x * v.x + self.y * v.y + self.z * v.z
         }
       
         fn Length(&self) -> f32
         {
             (self.x * self.x + self.y * self.y + self.z * self.z).sqrt()
         }
 
         fn Normalize(&self) -> Vector3
         {
             let len : f32 = self.Length();
             let x : f32 = self.x;
             let y : f32 = self.y;
             let z : f32 = self.z;
             if !IsZero(len)
             {
                 x /= len;
                 y /= len;
                 z /= len;
             }
             return Vector3::new(x, y, z);
         }
        
        //**********************************************************************
        // Define swizzle operators
        //**********************************************************************
        fn xyz(&self) -> Vector3
        {
            Self { x : self.x, y : self.y, z : self.z }
        }
        
        fn xzy(&self) -> Vector3
        {
            Self { x : self.x, y : self.z, z : self.y }
        }
        
        fn yxz(&self) -> Vector3
        {
            Self { x : self.y, y : self.x, z : self.z }
        }
        
        fn yzx(&self) -> Vector3
        {
            Self { x : self.y, y : self.z, z : self.x }
        }
        
        fn zyx(&self) -> Vector3
        {
            Self { x : self.z, y : self.y, z : self.x }
        }
        
        fn zxy(&self) -> Vector3
        {
            Self { x : self.z, y : self.x, z : self.y }
        }

        fn xxx(&self) -> Vector3
        {
            Self { x : self.x, y : self.x, z : self.x }
        }

        fn yyy(&self) -> Vector3
        {
            Self { x : self.y, y : self.y, z : self.y }
        }

        fn zzz(&self) -> Vector3
        {
            Self { x : self.z, y : self.z, z : self.z }
        }
    }
     
    impl ops::Add for Vector3
    {
        type Output = Vector3;
        fn add(self, _rhs : Vector3) -> Vector3
        {
            return Vector3::new(self.x + _rhs.x, self.y + _rhs.y, self.z + _rhs.z);
        }
    }
    
    impl ops::AddAssign for Vector3
    {
        fn add_assign(self : &mut Vector3, _rhs : Vector3) -> ()
        {
            *self = Self { x : self.x + _rhs.x, y : self.y + _rhs.y, z :  self.z + _rhs.z };
        }
    }
    
    impl ops::Sub for Vector3
    {
        type Output = Vector3;
        fn sub(self, _rhs : Vector3) -> Vector3
        {
            return Vector3::new(self.x - _rhs.x, self.y - _rhs.y, self.z - _rhs.z);
        }
    }
    
    impl ops::Neg for Vector3
    {
        type Output = Vector3;
        fn neg(self) -> Vector3
        {
            return Vector3::new(-self.x, -self.y, -self.z);
        }
    }
    
    impl ops::Mul<f32> for Vector3
    {
        type Output = Vector3;
        fn mul(self, f : f32) -> Vector3
        {
            return Vector3::new(self.x * f, self.y * f, self.z * f);
        }
    }

    impl ops::Div<f32> for Vector3
    {
        type Output = Vector3;
        fn div(self, f : f32) -> Vector3
        {
            return Vector3::new(self.x / f, self.y / f, self.z / f);
        }
    }

    impl ops::Add<f32> for Vector3
    {
        type Output = Vector3;
        fn add(self, f : f32) -> Vector3
        {
            return Vector3::new(self.x + f, self.y + f, self.z + f);
        }
    }

    impl ops::Sub<f32> for Vector3
    {
        type Output = Vector3;
        fn sub(self, f : f32) -> Vector3
        {
            return Vector3::new(self.x - f, self.y - f, self.z - f);
        }
    }

    impl PartialEq for Vector3
    {
        fn eq(&self, _rhs : &Self) -> bool
        {
            self.IsEq(*_rhs)
        }
        
        fn ne(&self, _rhs : &Self) -> bool
        {
            !self.IsEq(*_rhs)
        }
    }
}
