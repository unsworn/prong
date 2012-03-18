#ifndef buffer_H_
#define buffer_H_


class buffer
{
public:
    buffer();
    virtual ~buffer();

    void append(const char* data);

    unsigned length();

    unsigned size();

    void clear();

    char last();
    
    const char* get();
    
protected:
    char* alloc(unsigned size);
    
private:
    
    char* data;
};

#endif
