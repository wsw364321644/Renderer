#pragma once
#include <sigslot/signal.hpp>
#include <string>

/**
 * A CommonDelegate holds function callbacks.
 */

// Primary CommonDelegate template.
template<typename Func>
class CommonDelegate;

template<typename R, typename... Args>
class CommonDelegate<R( Args... )>
{
public:
    //using slot              = sigslot::slot<R(  )>;
    using signal = sigslot::signal<Args...>;
    using connection = sigslot::connection;
    //using scoped_connection = sig::scoped_connection;

    ///**
    // * Add function callback to the CommonDelegate.
    // *
    // * @param f The function to add to the CommonDelegate.
    // * @returns The connection object that can be used to remove the callback
    // * from the CommonDelegate.
    // */
    //template<typename Func, typename... Args>
    //connection operator+=( Func&& f, Args... args)
    //{
    //    return m_Callbacks.connect( std::forward<Func>( f ), std::forward<Args>(args)...);
    //}

    /**
     * Remove a callback function from the CommonDelegate.
     *
     * @param f The function to remove from the CommonDelegate.
     * @returns The number of callback functions removed.
     */
    template<typename Func>
    std::size_t operator-=( Func&& f )
    {
        return m_Callbacks.disconnect( std::forward<Func>( f ) );
    }

    /**
     * Invoke the CommonDelegate.
     */
    void operator()( Args... args )
    {
        return m_Callbacks( std::forward<Args>( args )... );
    }


    template<typename Func, typename ...Args>
    connection Bind(Func&& f, Args... args)
    {
        return m_Callbacks.connect(std::forward<Func>(f), std::forward<Args>(args)...);
    }

    void Reset()
    {
        return m_Callbacks.clear();
    }
private:
    signal m_Callbacks;
};

