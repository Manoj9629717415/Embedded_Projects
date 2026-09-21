file(REMOVE_RECURSE
  "VehicleHMI/qml/Main.qml"
  "VehicleHMI/qml/Settings.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/QTApp_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
