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
    private final double TO_DEG=(180/Math.PI);
    private final double TO_RAD=(Math.PI/180);
    private final int POW=2;
    private final double DEG=180.0; 
    private final double PIE=Math.PI;
    // constructors
    /**
     * Constructor for objects of class Point. Construct a new point with the specified x y coordinates. 
     * If the x coordinate is negative it is set to zero. If the y coordinate is negative it is set to zero.
     * @param x the x coordinate
     * @param y the y coordinate
     */
        public Point(double x, double y)
    {
        this._radius = calcRadiusFromXY(x, y);
        this._alpha= calcAlphaFromXY(x, y);
    }
    /** 
     * Constructor for objects of class Point. Copy constructor, construct a point using another point.
     * @param other The point from which to construct the new object
     */
        public Point(Point other)
    {
        if(other != null)
        {
            this._radius = other._radius;
            this._alpha = other._alpha;
        }
        else
        {
            this._radius = DEFAULT_VAL;
            this._alpha = DEFAULT_VAL;
        }
    }
    /**
     * Returns the radius of the a given point.
     * @param x the x coordinate
     * @param y the y coordinate
     */
    private double calcRadiusFromXY(double x, double y) 
    {
        return Math.sqrt(Math.pow(x,POW) + Math.pow(y,POW));
    }
    /**
     * Returns the angle of the given point. 
     * @param x the x coordinate
     * @param y the y coordinate
     */
    private double calcAlphaFromXY(double x, double y) 
    {
        return (Math.atan(y/x) * DEG) / PIE;
    }
    /**
     * This method rounds double number.
     * @param num
     * @return 
     */
    private double roundNumbers(double num)
    {
       return Math.round(num*10000)/(double)10000;
    }
    /**
     * This method returns the x coordinate of the point.
     * @return The x coordinate of the point
     */
    public double getX()
    {
        return roundNumbers(_radius * (Math.cos(_alpha*TO_RAD)));
    }
    /**
     * This method returns the y coordinate of the point.
     * @return The y coordinate of the point
     */
    public double getY()
    {
        return roundNumbers(_radius * (Math.sin(_alpha*TO_RAD)));
    }
    /**
     * This method sets the x coordinate of the point. If the new x coordinate is negative the old x coordinate will remain unchanged.
     * @param x The new x coordinate
     */
    public void setX(double x)
    {
        double y = this.getY();
        if (x>=DEFAULT_VAL)
        {
           _radius = Math.sqrt((y * y) + (x * x));
           _alpha = (Math.atan(y/x) * DEG) / PIE;
        }
    }
    /**
     * This method sets the y coordinate of the point. If the new y coordinate is negative the old y coordinate will remain unchanged.
     * @param y The new y coordinate
     */
    public void setY(double y)
    {
        double x = this.getX();
        if (y>=DEFAULT_VAL)
        {
           //_radius = roundNumbers(calcRadiusFromXY(getX(),y));
           //_alpha = roundNumbers(calcAlphaFromXY(getX(),y));
           _radius = Math.sqrt((y * y) + (x * x));
           _alpha = (Math.atan(y/x) * DEG) / PIE;
        }
    }
    /**
     * This method returns a string representation of Point in the format (x,y).
     * @overrides toString in class java.lang.Object
     * @return A String representation of the Point
     */
    public java.lang.String toString()
    {
        return "(" + (getX()) + "," + (getY()) + ")";
    }
    /**
     * This method checks if the given point is equal to this point.
     * @param other The point to check equality with
     * @return True if the given point is equal to this point
     */
    public boolean equals(Point other)
    {
        return (other.getX() == this.getX() && other.getY() == this.getY());
    }
    /**
    * This method checks if this point is above a received point.
    * @param other The point to check if this point is above
    * @return True if this point is above the other point
    */
    public boolean isAbove(Point other)
    {
        return (other.getY() < this.getY());
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
        return (this.getX() < other.getX());
    }
    /**
    * This method checks if this point is right of a received point.
    * @param other The point to check if this point is right of
    * @return True if this point is right of the other point
    */
    public boolean isRight(Point other)
    {
        return (other.isLeft(this));
    }
    /**
     * This method Moves a point. If either coordinate becomes negative the point remains unchanged.
     * @param dx The difference to add to x
     * @param dy The difference to add to y
     */
    public void move(double dx, double dy)
    {
        if ((getX() + dx) >= DEFAULT_VAL && (getY() + dy) >= DEFAULT_VAL)
        {
           setX((this.getX()+dx));
           setY((this.getY()+dy));
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
        return roundNumbers((Math.sqrt(Math.pow((other.getX() - this.getX()),POW) + Math.pow((other.getY() - this.getY()),POW))));
    }
}