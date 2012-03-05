#ifndef JSON_H_
#define JSON_H_

class JSONData
{
public:

    JSONData();
    virtual ~JSONData();

    void append(const char* name, const char* value);
    void append(const char* name, int value);
    void append(const char* name, double value);

    void open();    
    void close();
    
    const char* get();

    void write(const char*);
    
private:
    char* data;
};

#endif
