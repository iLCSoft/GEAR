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

	/** equals operator */
	bool operator == (const  GlobalPadIndex & cmp) const
	{
	  return (_moduleID==cmp._moduleID && _padIndex==cmp._padIndex);
	}

    /** Get pad index */
    virtual int getPadIndex() const;

    /** Get module index */
    virtual int getModuleID() const;

    /** Set the pad index */
    virtual void setPadIndex(int padIndex);

    /** Set the module ID */
    virtual void setModuleID(int moduleID);


}; // class
} // namespace gear


#endif /* ifndef GEAR_GlobalPadIndex_h */
