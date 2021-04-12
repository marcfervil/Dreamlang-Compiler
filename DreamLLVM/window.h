//
//  window.h
//  DreamLLVM
//
//  Created by Marc Fervil on 4/12/21.
//

#ifndef window_h
#define window_h
#include <objc/objc-runtime.h>


extern "C"{
/*
class Widget
{
  id wrapped;
public:
    Widget(id object);
 
    id object() const;
    Widget(const Widget &original) : Widget(original._object) {}
    virtual ~Widget();
       
protected:
    id _object;
};

*/
void mac_init(char * val);
 

}


#endif /* window_h */


/*
 
 

 #import <Foundation/Foundation.h>

 #import "window.h"

 extern "C"{
 void mac_init() {
   NSLog(@"test");
 }
 }

 **/
