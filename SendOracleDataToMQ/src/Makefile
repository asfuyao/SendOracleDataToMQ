CPP  = g++
RES  = 

OBJ  = mq.o datapack.o datatable.o logfile.o main.o $(RES)

LINKOBJ  = mq.o datapack.o datatable.o logfile.o main.o $(RES)

LIBS = -L /DBSoftware/app/oracle/product/10.2.0/db_1/lib -lclntsh -L /opt/mqm/lib -lmqm_r

CXXINCS = -I /DBSoftware/app/oracle/product/10.2.0/db_1/rdbms/public  -I /opt/mqm/inc 

BIN  = sendERPMessage

CXXFLAGS = $(CXXINCS)  

RM   = rm -f


$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "sendERPMessage" $(LIBS)

mq.o: mq.cpp
	$(CPP) -c mq.cpp -o mq.o $(CXXFLAGS)

datapack.o: datapack.cpp
	$(CPP) -c datapack.cpp -o datapack.o $(CXXFLAGS)

datatable.o: datatable.cpp
	$(CPP) -c datatable.cpp -o datatable.o $(CXXFLAGS)

logfile.o: logfile.cpp
	$(CPP) -c logfile.cpp -o logfile.o $(CXXFLAGS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

.PHONY: clean

clean:
	$(RM) $(OBJ) $(BIN)
