
export GEAR=$PWD

export GEARVERSION=v00-02

export PATH=$GEAR/tools:$GEAR/bin:$PATH
export JDK_HOME=/opt/products/java/1.4.2

#export GEARDEBUG=1


#----------- testing only: -------------

#----------- use RAIDA  -------------------------

export GEAR_USE_AIDA=1 

if [ $GEAR_USE_AIDA ] ; then
    export ROOTSYS=/opt/products/root/5.08.00
    export RAIDA_HOME=/afs/desy.de/user/g/gaede/ilcsoft/RAIDA/v00-00
    
    export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
    export PATH=$ROOTSYS/bin:$PATH
 
    . $RAIDA_HOME/bin/aida-setup.sh
fi
