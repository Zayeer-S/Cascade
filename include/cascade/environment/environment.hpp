#ifndef CASCADE_ENVIRONMENT_ENVIRONMENT_HPP
#define CASCADE_ENVIRONMENT_ENVIRONMENT_HPP

#include "cascade/utils/vector2d.hpp"

namespace cascade
{
    /**
     * @brief Wind conditions affecting fire spread
     */

    struct Wind
    {
        Vector2D direction;
        float speed;
        bool enabled;

        Wind() : direction(1.0f, 0.0f), speed(5.0f), enabled(true) {}

        Wind(const Vector2D &dir, float spd, bool en = true) : direction(dir.normalized()), speed(spd), enabled(en) {}
    };

    struct Weather
    {
        float temperature;
        float humidity;
        float precipitation;

        Weather() : temperature(25.0f), humidity(0.4f), precipitation(0.0f) {}
    };

    /**
     * @brief Environmental conditions for fire simulation
     *
     * Contains all environmental factors that affect fire behaviour:
     * wind, weather => MVP
     * terrain => post mvp TODO
     */
    class Environment
    {
    public:
        Environment() : wind_(), weather_() {}

        /**
         * @brief Update environmental conditions
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime)
        {
            (void)deltaTime;
        }

        /**
         * @brief Reset enviornment to default conditions
         */
        void reset()
        {
            wind_ = Wind();
            weather_ = Weather();
        }

        const Wind &getWind() const { return wind_; }
        Wind &getWind() { return wind_; }
        void setWind(const Wind &wind) { wind_ = wind; }

        const Weather &getWeather() const { return weather_; }
        Weather &getWeather() { return weather_; }
        void setWeather(const Weather &weather) { weather_ = weather; }

        Vector2D getWindDirection() const { return wind_.direction; }
        float getWindSpeed() const { return wind_.speed; }
        bool isWindEnabled() const { return wind_.enabled; }

        float getAmbientTemperature() const { return weather_.temperature; }
        float getHumidity() const { return weather_.humidity; }
        float getPrecipitation() const { return weather_.precipitation; }

    private:
        Wind wind_;
        Weather weather_;
    };

} // namespace cascade

#endif