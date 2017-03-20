#pragma once

#if 0
struct AwesomeApp;

struct StateIdle : public State<AwesomeApp>
{
    GET_SINGLETON_IMPL(StateIdle);

    void enter(AwesomeApp* app){}
    void update(AwesomeApp* app){}
    void draw(AwesomeApp* app){}
    void exit(AwesomeApp* app){}
};

struct StatePlay : public State<AwesomeApp>
{
    GET_SINGLETON_IMPL(StatePlay);

    void enter(AwesomeApp* app){}
    void update(AwesomeApp* app){}
    void draw(AwesomeApp* app){}
    void exit(AwesomeApp* app){}
};

struct AwesomeApp : public App,StateMachine<AwesomeApp>
{
    AwesomeApp(): StateMachine<AwesomeApp>(this)
    {
        getSignalUpdate().connect([&] {
            updateFSM();
        });

        getWindow()->getSignalDraw().connect([&] {
            drawFSM();
        });
    }
}
#endif

template <typename T>
struct State
{
    typedef std::shared_ptr<State<T>> Ref;

    virtual void enter(T* owner)    {};
    virtual void update(T* owner)   {};
    virtual void draw(T* owner)     {};
    virtual void exit(T* owner)     {};
    virtual void sendMessage(T* owner, const std::string& msg) {};
};

#define GET_SINGLETON_IMPL(classname) \
    static Ref get()\
{\
    static Ref sInstance = Ref(new classname);\
    return sInstance;\
}

template <typename T>
struct StateMachine
{
    typedef typename State<T>::Ref StateRef;
    StateRef mCurrentState;
    StateRef mPrevState;
    T* mOwner;

    StateMachine(T* owner)
    {
        mOwner = owner;
    };

    void updateFSM()
    {
        if (mCurrentState)
            mCurrentState->update(mOwner);
    }

    void drawFSM()
    {
        if (mCurrentState)
            mCurrentState->draw(mOwner);
    }

    void sendMessage(const std::string& msg)
    {
        if (mCurrentState)
            mCurrentState->sendMessage(mOwner, msg);
    }

    void changeToPreviousState()
    {
        changeToState(mOwner, mPrevState);
    }

    void changeToState(const StateRef& newState)
    {
        if (mCurrentState == newState)
            return;

        if (mCurrentState)
        {
            mPrevState = mCurrentState;
            mCurrentState->exit(mOwner);
        }
        mCurrentState = newState;
        mCurrentState->enter(mOwner);
    }
};
