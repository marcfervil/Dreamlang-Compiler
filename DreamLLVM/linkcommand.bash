#g++ -I/usr/local/Cellar/llvm/11.0.1/include -c -fPIC main.cpp -o "/Users/marcfervil/Documents/Programming/Dreamlang/lib/dream.o" -Wl,-no-as-needed -rdynamic -std=c++17 -F/usr/local/Cellar/llvm/11.0.1/lib  `llvm-config --cxxflags`  -fPIC 
#g++ -shared -WL,-install_name,dream.so -Wl,-undefined -Wl,dynamic_lookup -o "/Users/marcfervil/Documents/Programming/Dreamlang/lib/dream.so" "/Users/marcfervil/Documents/Programming/Dreamlang/lib/dream.o"  -lllvm -undefined dynamic_lookup -L/usr/local/opt/llvm/lib  -lz -ltermcap -lc++ -lLLVMCore -lLLVMSupport -lLLVMTransformUtils -lLLVMBitReader -lLLVMAnalysis -lLLVMDemangle -dynamiclib `llvm-config --ldflags -libs` -fPIC 
g++ -I/usr/local/Cellar/llvm/11.0.1/include -c -fPIC main.cpp -o dream.o -Wl,-no-as-needed -rdynamic -std=c++17 -F/usr/local/Cellar/llvm/11.0.1/lib  `llvm-config --cxxflags --ldflags --system-libs --libs all`  -fPIC 
g++ -shared -WL,-install_name,dream.so -Wl,-undefined -Wl,dynamic_lookup -o dream.so dream.o  -lllvm -undefined dynamic_lookup -L/usr/local/opt/llvm/lib  -lz -ltermcap -lc++ -lLLVMCore -lLLVMSupport -lLLVMTransformUtils -lLLVMBitReader -lLLVMAnalysis -lLLVMDemangle -dynamiclib `llvm-config --cxxflags --ldflags --system-libs --libs all` -fPIC 
cp dream.o "/Users/marcfervil/Documents/Programming/Dreamlang/lib/dream.o"
cp dream.so "/Users/marcfervil/Documents/Programming/Dreamlang/lib/dream.so"

#clang++ -I/usr/local/Cellar/llvm/11.0.1/include -F/usr/local/Cellar/llvm/11.0.1/lib lib/main.cpp lib/dream_output.o -o lib/program -o main
#clang++ main.cpp output.o -o main

#clang++ -L/usr/local/Cellar/llvm/11.0.1/lib -fPIC -isystem /usr/local/Cellar/llvm/11.0.1/include  -Wl,-undefined -Wl,dynamic_lookup  -fPIC -rdynamic lib/main.cpp  lib/dream_output.o -o lib/main -lllvm