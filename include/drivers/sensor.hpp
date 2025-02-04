#ifndef _driver_sensor_hpp_
#define _driver_sensor_hpp_

// #include <string>

// template<typename ratio>
// class unit
// {
//     private:
//     std::string id;
// };

// class volt : unit<ratio<1,1>>
// {

// };

// using volts = volt;

// class ohm : unit<ratio<1,1>>
// {

// };

// using ohms = ohm;
// {

// }

// auto voltage = 2 * volts;
// auto resistance = 3 * ohms;
// auto current = 6 * ampers;

// REQUIRE(current.unit() == (voltage * resistance).unit())
// REQUIRE(current.value() == (voltage * resistance).value())

// REQUIRE(current == (voltage * resistance))


template<typename type, typename ratio>
class sensor
{
    public:
    signal <sensor, type> value_changed;

    virtual type value_get() { return value; }

    virtual void value_set(type v) {

        if (value != v) {
            value = v;
            value_changed(*this, value);
        }

    }
    private:

    type value = 0;
};

#endif