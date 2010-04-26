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

}// namespace gear
