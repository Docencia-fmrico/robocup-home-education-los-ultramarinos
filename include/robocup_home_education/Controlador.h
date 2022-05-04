
// Copyright 2022 los ultramarinos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROBOCUP_HOME_EDUCATION_CONTROLADOR_H
#define ROBOCUP_HOME_EDUCATION_CONTROLADOR_H

#include <iostream>
#include <vector>

//#include "foo/stdio.h"
#include "robocup_home_education/PIDController.h"
#include "robocup_home_education/Controlador.h"

class Controlador
{
  public:
    Controlador();

    double errorGiro(double dg);
    double errorAvance(double da);
    double velocidadAngular(double errorAngular);
    double velocidadLineal(double errorAvance);

  protected:
    double distanciaMaxima;
    double distanciaSeguridad;


    std::vector<double> rangoAngular;
    std::vector<double> rangoLineal;


    std::vector<double> rangoAngularSalida;
    std::vector<double> rangoLinealSalida;

    PIDController PIDAngular;
    PIDController PIDLineal;

    double alfa;
};
#endif  // ROBOCUP_HOME_EDUCATION_CONTROLADOR_H
