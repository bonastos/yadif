from  __future__ import  print_function

import os
import sys
import re
import datetime
import string

versionParser = re.compile( r'(\s*Version\slibraryVersion)\s*\(\s*(.*)\s*,\s*(.*)\s*,\s*(.*)\s*,\s*\"(.*)\"\s*\).*' )
includesParser = re.compile( r'\s*#include\s*"(.*)"' )
guardParser = re.compile( r'\s*#.*YADIF_.*_HPP')
defineParser = re.compile( r'\s*#define')
ifParser = re.compile( r'\s*#ifndef YADIF_.*_HPP')
endIfParser = re.compile( r'\s*#endif//ndef YADIF_.*_HPP')
commentParser1 = re.compile( r'^\s*/\*')
commentParser2 = re.compile( r'^ \*')
blankParser = re.compile( r'^\s*$')
seenHeaders = set([])

yadifPath = os.path.dirname(os.path.realpath( os.path.dirname(sys.argv[0])))
rootPath = os.path.join( yadifPath, 'include/' )
versionPath = os.path.join( rootPath, "yadif/yadif_version.hpp" )
readmePath = os.path.join( yadifPath, "README.md" )
outputPath = os.path.join( yadifPath, 'single_header/yadif.hpp' )

bumpVersion = True
includeImpl = True

for arg in sys.argv[1:]:
    arg = string.lower(arg)
    if arg == "nobump":
        bumpVersion = False
        print( "Not bumping version number" )
    elif arg == "noimpl":
        includeImpl = False
        bumpVersion = False
        print( "Not including impl code (and not bumping version)" )
    else:
        print( "\n** Unrecognised argument: " + arg + " **\n" )
        exit(1)

out = open( outputPath, 'w' )
blanks = 0

def write( line ):
    out.write( line )

def parseFile( path, filename ):
    global blanks

    f = open( path + filename, 'r' )
    for line in f:
        m = includesParser.match( line )
        if m:
            header = m.group(1)
            headerPath, sep, headerFile = header.rpartition( "/" )
            if not headerFile in seenHeaders:
                seenHeaders.add( headerFile )
                parseFile( path + headerPath + sep, headerFile )
        else:
            if guardParser.match( line ) or defineParser.match( line ) or commentParser1.match( line ) or commentParser2.match( line ):
                line = ""
            if blankParser.match( line ):
                blanks = blanks + 1
            else:
                blanks = 0
            if blanks < 2:
                write( line.rstrip() + "\n" )

class Version:
    def __init__(self):
        f = open( versionPath, 'r' )
        for line in f:
            m = versionParser.match( line )
            if m:
                self.variableDecl = m.group(1)
                self.majorVersion = int(m.group(2))
                self.minorVersion = int(m.group(3))
                self.buildNumber = int(m.group(4))
                self.branchName = m.group(5)
        f.close()

    def incrementBuildNumber(self):
        self.buildNumber = self.buildNumber+1

    def updateVersionFile(self):
        f = open( versionPath, 'r' )
        lines = []
        for line in f:
            m = versionParser.match( line )
            if m:
                lines.append( '{0}( {1}, {2}, {3}, "{4}" );'.format( self.variableDecl, self.majorVersion, self.minorVersion, self.buildNumber, self.branchName ) )
            else:
                lines.append( line.rstrip() )
        f.close()
        f = open( versionPath, 'w' )
        for line in lines:
            f.write( line + "\n" )

    def updateReadmeFile(self):
        f = open( readmePath, 'r' )
        lines = []
        for line in f:
            lines.append( line.rstrip() )
        f.close()
        f = open( readmePath, 'w' )
        for line in lines:
            if line.startswith( "*v" ):
                f.write( '*v{0}.{1} build {2} ({3} branch)*\n'.format( self.majorVersion, self.minorVersion, self.buildNumber, self.branchName ) )
            else:
                f.write( line + "\n" )

def generateSingleHeader():
    v = Version()
    if bumpVersion:
        v.incrementBuildNumber()
        v.updateVersionFile()
        v.updateReadmeFile()
    out.write( "/*\n" )
    out.write( " *  YADIF v{0}.{1} build {2} ({3} branch)\n".format( v.majorVersion, v.minorVersion, v.buildNumber, v.branchName ) )
    out.write( " *  Generated: {0}\n".format( datetime.datetime.now() ) )
    out.write( " *  ----------------------------------------------------------\n" )
    out.write( " *  This file has been merged from multiple headers. Please don't edit it directly\n" )
    out.write( " *  Copyright (c) 2015 Dirk Bonekamper. All rights reserved.\n" )
    out.write( " *\n" )
    out.write( " *  Distributed under the Boost Software License, Version 1.0. (See accompanying\n" )
    out.write( " *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)\n" )
    out.write( " */\n\n" )
    out.write( "#ifndef YADIF_SINGLE_HEADER_HPP\n" )
    out.write( "#define YADIF_SINGLE_HEADER_HPP\n" )

    parseFile( rootPath, 'yadif.hpp' )

    out.write( "#endif//ndef YADIF_SINGLE_HEADER_HPP\n" )

generateSingleHeader()
