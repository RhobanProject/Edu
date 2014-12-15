#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <main/Command.h>
#include <rhoban/robot/Robots.h>


int main(int argc, char **argv)
{
    try {
        Rhoban::Robots robots;
	robots.loadYaml("config.yml");
        Rhoban::Robot* robot = robots["local"];
        robot->loadMove("/home/rhoban/Environments/RhobanServer/Mowgly/Moves/CartWalk.move.xml", true);
        robot->startMove("CartWalk", 0, 2000);

        /*
        <name>Frequency</name>
          <float>2.3</float>
        <name>Step</name>
          <float>0</float>
        <name>LateralStep</name>
          <float>0</float>
        <name>Rise</name>
          <float>2.3</float>
        <name>Turn</name>
          <float>0</float>
        <name>Swing</name>
          <float>0.4</float>
        <name>SwingPhase</name>
          <float>0</float>
        <name>SwingHeight</name>
          <float>0</float>
        <name>X</name>
          <float>1</float>
        <name>Y</name>
          <float>0.7</float>
        <name>Z</name>
          <float>3</float>
        <name>HipOffset</name>
          <float>-13</float>
        <name>YLat</name>
          <float>0</float>
        <name>SwingForce</name>
          <float>0</float>
        <name>RiseRatio</name>
          <float>0.5</float>
        <name>RiseStepPhase</name>
          <float>0</float>
        <name>IsEnabled</name>
          <float>0</float>
        */
        
        usleep(1000*1000*3);
        robot->updateConstant("CartWalk", "IsEnabled", 1);
        usleep(1000*1000*4);
        robot->updateConstant("CartWalk", "IsEnabled", 0);
        usleep(1000*1000*3);
        
        robot->stopMove("CartWalk", 2000);
	robots.stop();
    } catch (std::string err) {
        std::cout << "Exception : " << err << std::endl;
    }

    return 0;
}

