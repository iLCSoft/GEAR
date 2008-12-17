#ifndef GEAR_GlobalPadIndex_h
#define GEAR_GlobalPadIndex_h


namespace gear {

  /** Global pad index implimentation .
   *
   * @author M, Killenberg, (Bonn)  S, Turnbull, (Saclay/Carleton)
   * @version $Id: GlobalPadIndex.h,v 1.1 2008-12-17 09:39:46 engels Exp $ 
   */
	
class GlobalPadIndex  {

protected:
	int _padIndex;
	int _moduleID;

public:
	
	GlobalPadIndex(int padIndex, int moduleID);
	virtual ~GlobalPadIndex();

    /** Get pad index */
    virtual int getPadIndex() const;

    /** Get module index */
    virtual int getModuleID() const;


}; // class
} // namespace gear


#endif /* ifndef GEAR_GlobalPadIndex_h */
