

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_GPIOPIN1_INTERFACE_H_INCLUDE_GUARD
#define LE_GPIOPIN1_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_gpioPin1_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_gpioPin1_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_gpioPin1_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Pin polarities.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_GPIOPIN1_ACTIVE_HIGH = 0,
        ///< GPIO active-high, output is 1
    LE_GPIOPIN1_ACTIVE_LOW = 1
        ///< GPIO active-low, output is 0
}
le_gpioPin1_Polarity_t;


//--------------------------------------------------------------------------------------------------
/**
 * Edge transitions.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_GPIOPIN1_EDGE_NONE = 0,
        ///< No edge detection
    LE_GPIOPIN1_EDGE_RISING = 1,
        ///< Notify when voltage goes from low to high.
    LE_GPIOPIN1_EDGE_FALLING = 2,
        ///< Notify when voltage goes from high to low.
    LE_GPIOPIN1_EDGE_BOTH = 3
        ///< Notify when pin voltage changes state in either direction.
}
le_gpioPin1_Edge_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_gpioPin1_ChangeEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_gpioPin1_ChangeEventHandler* le_gpioPin1_ChangeEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Pull up or down.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_GPIOPIN1_PULL_OFF = 0,
        ///< Neither resistor is enabled
    LE_GPIOPIN1_PULL_DOWN = 1,
        ///< Pulldown resistor is enabled
    LE_GPIOPIN1_PULL_UP = 2
        ///< Pullup resistor is enabled
}
le_gpioPin1_PullUpDown_t;


//--------------------------------------------------------------------------------------------------
/**
 * State change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_gpioPin1_ChangeCallbackFunc_t)
(
    bool state,
        ///< New state of pin (true = active, false = inactive).
    void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Configure the pin as an input pin.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_SetInput
(
    le_gpioPin1_Polarity_t polarity
        ///< [IN] Active-high or active-low.
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure the pin as a push-pull output pin.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_SetPushPullOutput
(
    le_gpioPin1_Polarity_t polarity,
        ///< [IN] Active-high or active-low.
    bool value
        ///< [IN] Initial value to drive (true = active, false = inactive)
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure the pin as a tri-state output pin.
 *
 * @note The initial state will be high-impedance.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_SetTriStateOutput
(
    le_gpioPin1_Polarity_t polarity
        ///< [IN] Active-high or active-low.
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure the pin as an open-drain output pin.  "High" is a high-impedance state, while "Low"
 * pulls the pin to ground.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_SetOpenDrainOutput
(
    le_gpioPin1_Polarity_t polarity,
        ///< [IN] Active-high or active-low.
    bool value
        ///< [IN] Initial value to drive (true = active, false = inactive)
);



//--------------------------------------------------------------------------------------------------
/**
 * Enable the pull-up resistor (disables pull-down if previously enabled).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_EnablePullUp
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Enable the pull-down resistor (disables pull-up if previously enabled).
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_EnablePullDown
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Disable the pull-up and pull-down resistors.  Does nothing if both are already disabled.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_DisableResistors
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Set output pin to active state.
 *
 * @warning Only valid for output pins.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_Activate
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Set output pin to inactive state.
 *
 * @warning Only valid for output pins.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_Deactivate
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Set output pin to high impedance state.
 *
 * @warning Only valid for tri-state or open-drain output pins.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_SetHighZ
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Read value of GPIO input pin.
 *
 * @return true = active, false = inactive.
 *
 * @note It is invalid to read an output pin.
 */
//--------------------------------------------------------------------------------------------------
bool le_gpioPin1_Read
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_gpioPin1_ChangeEvent'
 *
 * Register a callback function to be called when an input pin changes state.
 *
 * If the pin is not capable of interrupt-driven operation, then it will be sampled every
 * sampleMs milliseconds.  Otherwise, sampleMs will be ignored.
 *
 * If this fails, either because the handler cannot be registered, or setting the
 * edge detection fails, then it will return a NULL reference.
 */
//--------------------------------------------------------------------------------------------------
le_gpioPin1_ChangeEventHandlerRef_t le_gpioPin1_AddChangeEventHandler
(
    le_gpioPin1_Edge_t trigger,
        ///< [IN] Change(s) that should trigger the callback to be called.
    le_gpioPin1_ChangeCallbackFunc_t handlerPtr,
        ///< [IN] The callback function.
    void* contextPtr,
        ///< [IN]
    int32_t sampleMs
        ///< [IN] If not interrupt capable, sample the input this often (ms).
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_gpioPin1_ChangeEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_gpioPin1_RemoveChangeEventHandler
(
    le_gpioPin1_ChangeEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the edge detection mode. This function can only be used when a handler is registered
 * in order to prevent interrupts being generated and not handled
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_SetEdgeSense
(
    le_gpioPin1_Edge_t trigger
        ///< [IN] Change(s) that should trigger the callback to be called.
);



//--------------------------------------------------------------------------------------------------
/**
 * Turn off edge detection
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_gpioPin1_DisableEdgeSense
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Check if the pin is configured as an output.
 *
 * @return true = output, false = input.
 */
//--------------------------------------------------------------------------------------------------
bool le_gpioPin1_IsOutput
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Check if the pin is configured as an input.
 *
 * @return true = input, false = output.
 */
//--------------------------------------------------------------------------------------------------
bool le_gpioPin1_IsInput
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of edge sensing.
 *
 * @return The current configured value
 *
 * @note it is invalid to read the edge sense of an output
 */
//--------------------------------------------------------------------------------------------------
le_gpioPin1_Edge_t le_gpioPin1_GetEdgeSense
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value the pin polarity.
 *
 * @return The current configured value
 */
//--------------------------------------------------------------------------------------------------
le_gpioPin1_Polarity_t le_gpioPin1_GetPolarity
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Check if the pin is currently active.
 *
 * @return true = active, false = inactive.
 *
 * @note this function can only be used on output pins
 */
//--------------------------------------------------------------------------------------------------
bool le_gpioPin1_IsActive
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of pull up and down resistors.
 *
 * @return The current configured value
 */
//--------------------------------------------------------------------------------------------------
le_gpioPin1_PullUpDown_t le_gpioPin1_GetPullUpDown
(
    void
);


#endif // LE_GPIOPIN1_INTERFACE_H_INCLUDE_GUARD