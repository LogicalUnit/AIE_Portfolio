#pragma once


//! The zSingleton class is a template class for creating singleton objects.
/*!
    When the static Instance() method is called for the first time, the singleton 
    object is created. Every sequential call returns a reference to this instance.
    The class instance can be destroyed by calling the DestroyInstance() method.
*/
template <class T> 
class zSingleton
{
public:
    
    //! Gets a reference to the instance of the singleton class.
    /*!
        \return A reference to the instance of the singleton class.
        If there is no instance of the class yet, one will be created.
    */
    static T* Instance()
    {
        if (m_instance == NULL) m_instance = new T;

//        assert(m_instance != NULL);

        return m_instance;
    };

    //! Destroys the singleton class instance.
    /*!
        Be aware that all references to the single class instance will be
        invalid after this method has been executed!
    */
    static int DestroyInstance()
    {
        delete m_instance;
        m_instance = NULL;
		return 0;
    };

protected:

    // shield the constructor and destructor to prevent outside sources
    // from creating or destroying a zSingleton instance.

    //! Default constructor.
    zSingleton()
    {
    };


    //! Destructor.
    virtual ~zSingleton()
    {
    };

private:

    //! Copy constructor.
    zSingleton(const zSingleton& source)
    {
    };

    static T* m_instance; //!< singleton class instance
};

//! static class member initialisation.
template <class  T> T* zSingleton<T>::m_instance = NULL;
