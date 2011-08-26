#ifndef GEAR_GlobalPadIndex_h
#define GEAR_GlobalPadIndex_h


namespace gear {

  /** Global pad index implimentation .
   *
   * @author M, Killenberg, (Bonn)  S, Turnbull, (Saclay/Carleton)
   * @version $Id$ 
   */
	
class GlobalPadIndex  {

protected:
	int _padIndex;
	int _moduleID;

public:
	
	GlobalPadIndex(int padIndex, int moduleID);
	virtual ~GlobalPadIndex();

	bool operator < (const  GlobalPadIndex & cmp) const
	{
	  if (this->getModuleID() == cmp.getModuleID() )
	    { // the modules IDs are equal, distiguish using pad index
	      return ( this->getPadIndex() < cmp.getPadIndex() );
	    }
	  else 
	    { // the modules IDs are equal, distiguish using pad index
	      return ( this->getModuleID() < cmp.getModuleID() );
	    }
	}

    /** Get pad index */
    virtual int getPadIndex() const;

    /** Get module index */
    virtual int getModuleID() const;


}; // class
} // namespace gear


#endif /* ifndef GEAR_GlobalPadIndex_h */
