
export GEAR=$PWD

export GEARVERSION=v00-08-01

export PATH=$GEAR/tools:$GEAR/bin:$PATH
export JDK_HOME=/afs/desy.de/group/it/ilcsoft/java/1.6.0

#export GEARDEBUG=1


#----------- testing only: -------------

#----------- use RAIDA  -------------------------

export GEAR_USE_AIDA=1

if [ $GEAR_USE_AIDA ] ; then
    export ROOTSYS=/afs/desy.de/group/it/ilcsoft/root/5.08.00
    export RAIDA_HOME=/afs/desy.de/group/it/ilcsoft/v01-03-03/RAIDA/v01-03
    
    export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
    export PATH=$ROOTSYS/bin:$PATH
 
    . $RAIDA_HOME/bin/aida-setup.sh
fi


#------------use CGA -------------------------

#export GEAR_USE_CGA=1
unset GEAR_USE_CGA

if [ $GEAR_USE_CGA ] ; then
# Define the folowing variables for gearcga:

#---- test setup --------------- 
#export MOKKA=/home/musat/Mokka
#export MYSQLLIB=/usr/lib/mysql
#export CLHEP_BASE_DIR=/usr/local/CLHEP/2.0.2.2
#export G4INSTALL=/usr/local/geant4/geant4.8.0.p01
#export G4SYSTEM=Linux-g++
#export MOKKALIBS=$HOME/$G4INSTALL/tmp/$G4SYSTEM
#export LCIO=/usr/local/LCIO/01.07

#---- DESY setup --------------- 
export MOKKA=/afs/desy.de/group/it/ilcsoft/mokka/6.3.p01
export MYSQLLIB=/afs/desy.de/group/it/ilcsoft/MySQL/5.0.26/lib/mysql
export CLHEP_BASE_DIR=/afs/desy.de/group/it/ilcsoft/CLHEP/2.0.2.2
export G4INSTALL=/afs/desy.de/group/it/ilcsoft/geant4/8.0.p01
export G4SYSTEM=Linux-g++
export MOKKALIBS=$MOKKA/tmp/$G4SYSTEM
export LCIO=/afs/desy.de/group/it/ilcsoft/lcio/v01-08-02
fi


