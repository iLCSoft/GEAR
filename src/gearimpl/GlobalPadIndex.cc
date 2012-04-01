#include "gearimpl/GlobalPadIndex.h"

namespace gear {

    GlobalPadIndex::GlobalPadIndex(int padIndex, int moduleID):
	_padIndex( padIndex) ,
	_moduleID (moduleID) {
    }
    GlobalPadIndex::~GlobalPadIndex(){
    }
    int GlobalPadIndex::getPadIndex() const {
	return _padIndex;
    }
    int GlobalPadIndex::getModuleID() const {
	return _moduleID;
    }
    void GlobalPadIndex::setPadIndex(int padIndex) {
      _padIndex=padIndex;
    }
    void GlobalPadIndex::setModuleID(int moduleID) {
      _moduleID=moduleID;
    }

}// namespace gear
