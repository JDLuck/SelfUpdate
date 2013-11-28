 SelfUpdateModule

 A Simple and effective, modular, self updating class.
    Copyright (C) 2013  Jerry Luck

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
  
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

 INSTRUCTIONS:
 The things you need are;
	-A http capable server 
	-The fciv.exe from microsoft (provided)
	-An xml file generated from fciv.exe on your web server
	-And of course you files you need to update, also stored on your server

 NOTES:
 I've found this strange curl compile error:
 libcurl Expression: _CrtIsValidHeapPointer(pUserData)
 The fix is:
 Properties->Configuration Properties->General->Whole Program Optimisation->Use link Time Code Generation

 PLEASE NOTE THIS CURRENT VERSION HAS NOT BEEN TESTED WITH A SELF REPLACING EXE (see SelfUpdateModule::XML_updateEXE())
 SO IT WILL PROBABLY NOT WORK! 