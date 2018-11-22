/**
 * This class represents 2 dimensional points.
 *
 * @author Noy Meir
 * @version 21/11/2018
 */
public class Point
{
    // declarations 
    private double _radius;
    private double _alpha;
    private final double DEFAULT_VAL=0.0;
    // constructors
    /**
     * Constructor for objects of class Point. Construct a new point with the specified x y coordinates. 
     * If the x coordinate is negative it is set to zero. If the y coordinate is negative it is set to zero.
     * @param x the x coordinate
     * @param y the y coordinate
     */
    public Point(double x, double y)
    {
        _radius= DEFAULT_VAL;
        _alpha= DEFAULT_VAL;
        //x = _radius * (Math.cos(_alpha));
        //y = _radius * (Math.sin(_alpha));
    }
    /**
     * Constructor for objects of class Point. Copy constructor, construct a point using another point.
     * @param other The point from which to construct the new object
     */
    public Point (Point other)
    {   
        if(other != null)
        {
            //x=other.x;
            //y=other.y;
        }
        else
        {   
            //_x=DEFAULT_VAL;
            //_y=DEFAULT_VAL;
        }
    }
    
    /**
     * This method returns the x coordinate of the point.
     * @return The x coordinate of the point
     */
    public double getX()
    {
        return (Math.round((_radius * (Math.cos(_alpha)))*10000)/(double)10000);
    }
    /**
     * This method returns the y coordinate of the point.
     * @return The y coordinate of the point
     */
    public double getY()
    {
        return (Math.round((_radius * (Math.sin(_alpha)))*10000)/(double)10000);
    }
    /**
     * This method sets the x coordinate of the point. If the new x coordinate is negative the old x coordinate will remain unchanged.
     * @param x The new x coordinate
     */
    public void setX(double x)
    {
        if (x>=DEFAULT_VAL)
        {
            //_x=x;
        }
    }
    /**
     * This method sets the y coordinate of the point. If the new y coordinate is negative the old y coordinate will remain unchanged.
     * @param y The new y coordinate
     */
    public void setY(double y)
    {
        if (y>=DEFAULT_VAL)
        {
            //_y=y;
        }
    }
    /**
     * This method returns a string representation of Point in the format (x,y).
     * @overrides toString in class java.lang.Object
     * @return A String representation of the Point
     */
    public java.lang.String toString()
    {
        return "(" + (getY()) + "," + (getX()) + ")";
    }
    /**
     * This method checks if the given point is equal to this point.
     * @param other The point to check equality with
     * @return True if the given point is equal to this point
     */
    public boolean equals(Point other)
    {
        return (other.getX() == getX() && other.getY() == getY());
    }
    /**
    * This method checks if this point is above a received point.
    * @param other The point to check if this point is above
    * @return True if this point is above the other point
    */
    public boolean isAbove(Point other)
    {
        //return (other._y < _y);
    }
    /**
    * This method checks if this point is below a received point.
    * @param other The point to check if this point is below
    * @return True if this point is below the other point
    */
    public boolean isUnder(Point other)
    {
        return (other.isAbove(this));
    }
    /**
    * This method checks if this point is left of a received point.
    * @param other The point to check if this point is left of
    * @return True if this point is left of the other point
    */
    public boolean isLeft(Point other)
    {
        //return (_x < other._x);
    }
    /**
    * This method checks if this point is right of a received point.
    * @param other The point to check if this point is right of
    * @return True if this point is right of the other point
    */
    public boolean isRight(Point other)
    {
        //return (other.isLeft(this));
    }
    /**
     * This method Moves a point. If either coordinate becomes negative the point remains unchanged.
     * @param dx The difference to add to x
     * @param dy The difference to add to y
     */
    public void move(double dx, double dy)
    {
        if ((_x + dx) >= DEFAULT_VAL || (_y + dy) >= DEFAULT_VAL)
        {
           //_x = _x + dx;
           //_y = _y + dy;
        }
    }
    /**
     * Check the distance between this point and a given point.
     * Parameters:
     * @param other - The point to check the distance from
     * @return - The distance
     */
    public double distance(Point other)
    {
        //return(Math.sqrt(Math.pow((other.x - _x),2) + Math.pow((other.y - _y),2)));
        //return(     Math.sqrt(      Math.pow((other._y - _y),2)     +       Math.pow((other._x -_x),2)        )          );
    }
}