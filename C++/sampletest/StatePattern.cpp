#include <iostream>


class EcuContext;


class ECUstate{
public:
    virtual ~ECUstate() = default;
    virtual void handle(ECUContext& context) = 0;
};

class InitState : public ECUstate{
public:
    void handle(ECUContext& context)
    {
        context.startEngine();
    }
};

class RuuningState : public ECUstate{
public:
    void handle(ECUContext& context)
    {
        context.selectDrivemode();
    }

};

class ErrorState : publlic ECUstate{
public:
    void handle(ECUContext& context)
    {
        context.logerror();
    }

};

class ECUContext{
public:
    void setState(ECUstate& state)
    {
        m_state = state;
    }

    void handle()
    {
        m_state->handle(*this);
    }

    void startEngine(){
        std::cout<<"Engine started \n";
    }

     void selectDrivemode(){
        std::cout<<"selectDrivemode \n";
    }

     void logerror(){
        std::cout<<"logerror \n";
    }

private:
    ECUstate* m_state;
};

class SmartSingleTon{


private:
    static std::unique

};
