
export GEAR=$PWD

export GEARVERSION=v00-02

export PATH=$GEAR/tools:$GEAR/bin:$PATH
export JDK_HOME=/usr

#export GEARDEBUG=1


#----------- testing only: -------------

#----------- use RAIDA  -------------------------

#export GEAR_USE_AIDA=1

if [ $GEAR_USE_AIDA ] ; then
    export ROOTSYS=/opt/products/root/5.08.00
    export RAIDA_HOME=/afs/desy.de/user/g/gaede/ilcsoft/RAIDA/v00-00
    
    export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
    export PATH=$ROOTSYS/bin:$PATH
 
    . $RAIDA_HOME/bin/aida-setup.sh
fi


#------------use CGA -------------------------

export GEAR_USE_CGA=1

if [ $GEAR_USE_CGA ] ; then
# Define the folowing variables for gearcga:
export MOKKA=/home/musat/Mokka
export MYSQLLIB=/usr/lib/mysql
export CLHEP_BASE_DIR=/usr/local/CLHEP/2.0.2.2
export G4INSTALL=/usr/local/geant4/geant4.8.0.p01
export G4SYSTEM=Linux-g++
export MOKKALIBS=$HOME/$G4INSTALL/tmp/$G4SYSTEM
# Define the folowing variable if Mokka was compiled in LCIO mode:
export LCIO=/usr/local/LCIO/01.07
fi


