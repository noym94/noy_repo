
/**
 * Represents information about a city Maman12 question2 2019a.
 *
 * @author Noy Meir
 * @version 23.11.2018
 */

public class City
{
    // instance variables - replace the example below with your own
    private String _cityName;
    private Point _cityCenter;
    private Point _centralStation;
    private long _numOfResidents; 
    private int _noOfNeighborhoods;  
    public final int MIN_NEIGBORHOODS= 0;
    public final long MIN_RESIDENTS = 0;
    public final int NEIGBORHOOD_DEAFAULT = 1;
    /**
     * Constructor for objects of class City. Copy constructor, construct a city using another city.
     * @param other - The city from which to construct the new object
     */
    public City(City other)
    {
        this._cityName = other._cityName;
        this._cityCenter = other._cityCenter;
        this._centralStation = other._centralStation;
        this._numOfResidents = other._numOfResidents;
        this._noOfNeighborhoods = other._noOfNeighborhoods;
    }
    /**
     * Constructor for objects of class City. Construct a new city with the specified info. If numOfResidents is smaller than MIN_RESIDENTS, MIN_RESIDENTS is used. If noOfNeighborhoods is smaller than MIN_NEIGBORHOODS, NEIGBORHOOD_DEAFAULT is used. If any coordinate is negative - 0 is used instead.
     * @param cityName - The name of the city
     * @param cityCenterX - The X coordinate of the center point of the city
     * @param cityCenterY - The Y coordinate of the center point of the city
     * @param centralStationX - The X coordinate of the central station of the city
     * @param centralStationY - The Y coordinate of the central station of the city
     * @param numOfResidents - The number of residents in the city
     * @param noOfNeighborhoods - The number of neighborhoods in the city
     */
    public City(java.lang.String cityName,
            double cityCenterX,
            double cityCenterY,
            double centralStationX,
            double centralStationY,
           long numOfResidents,
            int noOfNeighborhoods)
    {
        this._cityName = cityName;
        this._cityCenter= new Point (cityCenterX,cityCenterY);
        this._centralStation= new Point (centralStationX,centralStationY);
        if (numOfResidents >= MIN_RESIDENTS)
        {
            this._numOfResidents=numOfResidents;
        }
        else
        {
             this._numOfResidents = MIN_RESIDENTS;
        }
        if (noOfNeighborhoods > MIN_NEIGBORHOODS)
        {
            this._noOfNeighborhoods=noOfNeighborhoods;
        }
        else
        {
            this._noOfNeighborhoods=NEIGBORHOOD_DEAFAULT;
        }
    }
    /**
     * Adds the given number of residents - either positive or negative number - to the city. If the resulted number of residents is smaller than MIN_RESIDENTS, MIN_RESIDENTS is set to be the new number of residents of the city, and false is returned. Otherwise (i.e. the resulted number is bigger or equal to MIN_RESIDENTS), true is returned.
     * Returns false if resulted number of residents is smaller than MIN_RESIDENTS, true otherwise
     * @param  
     * @return    
     */
    public boolean addResidents(long residentsUpdate)
    {
        if ((this._numOfResidents + residentsUpdate) < MIN_RESIDENTS)
        {
            setNumOfResidents(MIN_RESIDENTS);
            return false;
        }
        else
        {
            setNumOfResidents(this._numOfResidents + residentsUpdate);
            return true;
        }
    }
    /**
     * Calculates the distance between the center of this city and its central station.
     * Returns The distance between the center of this city and its central station
     * @return   
     */
    public double distanceBetweenCenterAndStation()
    {
        Point p0 = new Point(this._centralStation);
        Point p1 = new Point(this._cityCenter);
        return p1.distance(p0);
    }
    /**
     * Returns the centeral station of the city as a Point object.
     * @return   
     */
    public Point getCentralStation()
    {
        return this._centralStation;
    }
    /**
     * Returns the center of the city as a Point object.
     * @param    
     * @return    
     */
    public Point getCityCenter()
    {
        return this._cityCenter;
    }
    /**
     * Returns the name of the city.
     * @param  
     * @return    
     */
    public java.lang.String getCityName()

    {
        return this._cityName;
    }
    /**
     * Returns the number of neighborhoods of the city.
     * @param  
     * @return    
     */
    public int getNoOfNeighborhoods()
    {
        return this._noOfNeighborhoods;
    }
    /**
     * Returns the number of residents of the city.
     * @param  
     * @return    
     */
    public long getNumOfResidents()
    {
        return this._numOfResidents;
    }
    /**
     * Move the location of the central station of the city with the given deltas. If the new location has a negative coordinate - the central station keeps its original location.
     * @param  deltaX - How much the x coordinate of the central station of the city is to be moved
     * @param deltaY - How much the y coordinate of the central station of the city is to be moved    
     */
    public void moveCentralStation(double deltaX,
                                   double deltaY)

    {
        Point p0 = new Point(this._centralStation);
        p0.move(deltaX, deltaY);
        setCentralStation(p0);
    }
    /**
     * Creates a new city with a new name, and where its center and central station are moved by the given deltas from this city. If either new point has negative coordinates - it gets the center or central station of this city. Its number of residents is set to MIN_RESIDENTS and its number of neightborhoods is set to MIN_NEIGBORHOODS.
     * @param  newCityName - The name of the new city
     * @param dx - How much the x coordinates of the city's center and central station are to be moved for the new city
     * @param dy - How much the y coordinates of the city's center and central station are to be moved for the new city
     * @return    
     */
    public City newCity(java.lang.String newCityName,
                    double dX,
                    double dY)
    {
      
        Point newCentralStation = new Point(getCentralStation());
        newCentralStation.move(dX, dY);
        
        Point newCityCenter = new Point(getCityCenter());
        newCityCenter.move(dX, dY);
        
        City newCity = new City(
            newCityName, 
            newCityCenter.getX(),
            newCityCenter.getY(),
            newCentralStation.getX(),
            newCentralStation.getY(),
            MIN_RESIDENTS, 
            NEIGBORHOOD_DEAFAULT);
        return newCity;
    }
    /**
     * Sets the central station point of the city.
     * @param  centralStation - The new central station point of the city
     */
    public void setCentralStation(Point centralStation)
    {
        this._centralStation = new Point(centralStation);
    }
    /**
     * Sets the center point of the city.
     * @param  cityCenter - The new center point of the city
     */
    public void setCityCenter(Point cityCenter)
    {
       this._cityCenter = new Point(cityCenter);
    }
     /**
     * Sets the name of the city.
     * @param  cityName - The new name of the city
     */
    public void setCityName(java.lang.String cityName)
    {
        this._cityName = cityName;
    }
    /**
     * Sets the number of neighborhoods of the city, but only if it is bigger or equal to MIN_NEIGBORHOODS.
     * @param  noOfNeighborhoods - The new number of neighborhoods in the city
     */
    public void setNoOfNeighborhoods(int noOfNeighborhoods)
    {
        if (noOfNeighborhoods >= MIN_NEIGBORHOODS)
        {
            this._noOfNeighborhoods = noOfNeighborhoods;
        }
    }
    /**
     * Sets the number of residents of the city, but only if it is bigger to MIN_RESIDENTS.
     * @param  numOfResidents - The new number of residents in the city
     */
    public void setNumOfResidents(long numOfResidents)
    {
        if (numOfResidents > MIN_RESIDENTS)
        {
            this._numOfResidents = numOfResidents;
        }
    }
    /**
     * Returns a string representation of this City in the format
     *          City Name: Tel Aviv
     *          City Center: (5.0,8.0)
     *          Central Station: (3.0,4.0)
     *          Number of Residents: 1004
     *          Number of Neighborhoods: 5
     * @override  toString in class java.lang.Object

     * @return    A String representation of this city
     */
    public java.lang.String toString()
    {
        Point p0 = new Point(getCentralStation());
        Point p1 = new Point(getCityCenter());
        
        return "City Name: " + (getCityName()) + "\n" 
        + "City Center: " + (p1.toString()) + "\n" 
        + "Central Station: " + (p0.toString()) + "\n" 
        + "Number of Residents: " + (getNumOfResidents()) + "\n"
        + "Number of Neighborhoods: " + (getNoOfNeighborhoods()) + "\n";
    }
}
