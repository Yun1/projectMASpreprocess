#!/usr/bin/env python

from __future__ import print_function
import subprocess
import os



# set project base directory structure
base = os.getcwd()
    
try:
    from setuptools import setup
    setup_kwargs = {'entry_points': {'console_scripts':['preparepydisalexi=prepare_disalexi.processLandsatLAI:main']}}
except ImportError:
    from distutils.core import setup
    setup_kwargs = {'scripts': ['bin/preparepydisalexi']}
    
from preparepydisalexi import __version__

#=====build DMS binaries===============================
# get Anaconda root location
p = subprocess.Popen(["conda", "info", "--root"],stdout=subprocess.PIPE)
out = p.communicate()
condaPath = out[0][:-1]
libEnv = os.path.join(condaPath,'lib')    

processDir = base
libDir = os.path.join(processDir,'source','lib')


#====Creating SYMBOLIC L:INKS===========
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libtiff.a'), 
"%s" % os.path.join(libDir,'libtiff.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'liblzma.a'), 
"%s" % os.path.join(libDir,'liblzma.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libjpeg.a'), 
"%s" % os.path.join(libDir,'libjpeg.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libgeotiff.a'), 
"%s" % os.path.join(libDir,'libgeotiff.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libsz.a'), 
"%s" % os.path.join(libDir,'libsz.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libGctp.a'), 
"%s" % os.path.join(libDir,'libGctp.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libdf.a'), 
"%s" % os.path.join(libDir,'libdf.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libz.a'), 
"%s" % os.path.join(libDir,'libz.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libmfhdf.a'), 
"%s" % os.path.join(libDir,'libmfhdf.a')])
subprocess.call(["ln","-s", "%s" % os.path.join(libEnv,'libhdfeos.a'), 
"%s" % os.path.join(libDir,'libhdfeos.a')])

mkPath = os.path.join(processDir,'source','Landsat_LAI','lndlai_compute')
os.chdir(mkPath)
subprocess.call(["scons","-Q","install"])
subprocess.call(["scons","-c"])

mkPath = os.path.join(processDir,'source','Landsat_LAI','lndlai_sample')
os.chdir(mkPath)
subprocess.call(["scons","-Q","install"])
subprocess.call(["scons","-c"])


mkPath = os.path.join(processDir,'source','Landsat_LAI','GeoTiff2ENVI')
os.chdir(mkPath)
subprocess.call(["scons","-Q","install"])
subprocess.call(["scons","-c"])


mkPath = os.path.join(processDir,'source','Landsat_DMS')
os.chdir(mkPath)
subprocess.call(["scons","-Q","install"])
subprocess.call(["scons","-c"])

mkPath = os.path.join(processDir,'source','MODIS_DMS')
os.chdir(mkPath)
subprocess.call(["scons","-Q","install"])
subprocess.call(["scons","-c"])

mkPath = os.path.join(processDir,'source','Cubist')
os.chdir(mkPath)
subprocess.call(["scons","-Q","install"])
subprocess.call(["scons","-c"])

#=============setup the python scripts============================



setup(
    name="projectmaspreprocess",
    version=__version__,
    description="prepare data for input to pyDisALEXI",
    author="Mitchell Schull",
    author_email="mitch.schull@noaa.gov",
    url="https://github.com/bucricket/projectMASpreprocess.git",
    packages= ['prepare_disalexi'],
    platforms='Posix; MacOS X; Windows',
    license='BSD 3-Clause',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2',
        # Uses dictionary comprehensions ==> 2.7 only
        'Programming Language :: Python :: 2.7',
        'Topic :: Scientific/Engineering :: GIS',
    ],  
    **setup_kwargs
)