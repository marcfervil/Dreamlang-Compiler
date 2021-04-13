//
//  window.h
//  DreamLLVM
//
//  Created by Marc Fervil on 4/12/21.
//

//#import <Foundation/NSString.h>
#include <objc/objc-runtime.h>

//#include <Foundation/Foundation.h>
#ifndef window_h
#define window_h






extern "C"{

class Widget
{
  id wrapped;
    public:
        Widget(id object);
        id object() const;
        Widget(const Widget &original) : Widget(original._object) {}
     /*
        id object() const;
        Widget(const Widget &original) : Widget(original._object) {}
        virtual ~Widget();*/
           
    
    void * operator new(size_t n);
   
    void operator delete(void * p); // or delete(void *, std::size_t)
    
    void start();
    
    protected:
        id _object;
    
   
};


Widget *  mac_init(char * val);

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
