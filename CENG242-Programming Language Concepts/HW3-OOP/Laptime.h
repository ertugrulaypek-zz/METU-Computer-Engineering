#ifndef HW3_LAPTIME_H
#define HW3_LAPTIME_H


#include <ostream>


class Laptime {

private:
    int laptime;

    Laptime *next;

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PRIVATE METHODS/PROPERTIES BELOW

public:
    /**
     * Constructor.
     *
     * @param int value in laptime.
     */
    Laptime(int laptime);

    /**
     * Copy Constructor.
     *
     * @param rhs The laptime to be copied.
     */
    Laptime(const Laptime& rhs);

    ~Laptime();

    /**
     * Sets the next chain for this Laptime.
     *
     *
     * @param next The next Laptime.
     */
    void addLaptime(Laptime *next);

    /**
     * Less than overload.
     *
     * True if this Laptime less than the rhs Laptime.
     *
     * @param rhs The Laptime to compare.
     * @return True if this laptime is smaller, false otherwise.
     */
    bool operator<(const Laptime& rhs) const;

    /**
     * Greater than overload.
     *
     * True if this Laptime greater than the rhs Laptime.
     *
     * @param rhs The Laptime to compare.
     * @return True if this laptime is bigger, false otherwise.
     */
    bool operator>(const Laptime& rhs) const;
    
    /**
     * Indexing.
     *
     * Find the Laptime in desired position(start from zero).
     *
     * @return The Laptime with the given lap. Laptime with zero time if given lap does not exists.
     */
    //Laptime operator[](const int lap) const;

    /**
     *  Plus overload
     * 
     *  Add two Laptime and return another Laptime
     * 
     *  @param Laptime to add
     *  @returns Summation of the two laptime
     */
    Laptime& operator+(const Laptime& rhs);

    /**
     * Stream overload.
     *
     * What to stream:
     * minute:second.miliseconds
     *
     * Example:
     * 1:19.125
     *
     * @important Your laptime variable is representation in terms of miliseconds 
     * and you have to turn it to desired outcome type
     *
     * @param os Stream to be used.
     * @param laptime Laptime to be streamed.
     * @return The current Stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Laptime& laptime);

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES BELOW

	Laptime* getNext(){
		return this->next;
	}
	int getLaptime() const{
		return this->laptime;
	}
	void setNext(Laptime* rhs){
		this->next=rhs;
	}
	
};



#endif //HW3_LAPTIME_H
