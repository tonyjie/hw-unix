JSONCPP_DIST_PATH=./dist
all:
	g++ main.cpp lenet.cpp layer.cpp mnist.cpp -o main -I ${JSONCPP_DIST_PATH} `pkg-config opencv --cflags --libs`
	./main
clean:
	rm ./main