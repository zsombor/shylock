HOW TO BUILD:

    If building a release version, make sure that the following files are in
the same directory as the Shylock executable:

     -params.bin                  SEARCH PARAMETERS
     -book.bin                    BOOK           
     -Usage.htm                   HELP FILE
     -shylock.css                 HELP FILE STYLE DEFINITION
     -logo1.jpg                   SHYLOCK LOGO
     -background1.jpg             HELP BACKGROUND

	The *.bin files can be copied from a binary install, you will find them 
in your installation directory. The reason for this separation is the size 
of these files, and some limitations imposed by bandwidth of free webhosting 
services. Lacking the  file "params.bin" will result in a runtime exception. 
Whilest the file "book.bin" is optional, the program can perform without an 
opening book, however playing strength will reduced.
    A release version must be built with optimizations (especially inlining)
turned on and with the NDEBUG macro defined. Failing to do so will decrease
search speed to less than 10% due to a large amount of assert() calls. If 
building with VC++ the STRICT macro must be set in the project options. There 
should be no problems if using the supplied project file. Ofcourse if you have 
made modifications to the source, make sure that no assert() calls will fail
after several games. An automated test framework would be welcomed, however 
due to the multitude of positions generated and analized during the search with
assert() calls turned on, problems become evident easily.
   The setup kits can be built with the NSIS system from www.nullsoft.com all
setup related credits to them. Use the Doxygen system from www.doxygen.org 
to generate browsable class documentation. 

NOTE:

    You are free to modify this code as you please, but you may not obscure
it's origin. Restrictions from the file license.txt are to be applied. I hold
no right upon the code from the src/lib directory. There you will find a bunch
of reusable classes, mostly windows specific (except some CRC related) from 
the win32 API tutorials from www.relisoft.com. There is a project called 
WinLib with the specific task of encapsulating the win32 api in a nice and 
easy to use C++ class set. You may wish to check the latest state of WinLib,
by using the Code Co-op version control system from www.relisoft.com. All 
domain specific classes, implementing the Othello knowledge are to be found 
in the src/model directory, enclosed in the Othello namespace. The Shylock 
logo was created using the GIMP's (see http://www.gimp.org) scripts.


Enjoy!
