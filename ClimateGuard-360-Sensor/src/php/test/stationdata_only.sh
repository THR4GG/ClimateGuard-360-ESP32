curl -v -X POST http://climateguard-360.local:8080/insert_data.php \
    -d "station_id=00:1A:22:33:44:55" \
    -d "station_name=TestStation" \
    -d "station_description=TestDescription" \
    -d "station_mode=Outdoor"
