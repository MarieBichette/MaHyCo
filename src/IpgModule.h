﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
#ifndef IPGMODULE_H
#define IPGMODULE_H

#include "Ipg_axl.h"

using namespace Arcane;

/**
 * Représente un module pour le traitement des particules
 * - création de particules avec une vitesse donnée
 * - calcul de la trajectoire des particules
 * 
 */


class IpgModule
: public ArcaneIpgObject
{
 public:
  /** Constructeur de la classe */
  IpgModule(const ModuleBuildInfo& mbi) : ArcaneIpgObject(mbi) {}
  
  /** Destructeur de la classe */
  ~IpgModule() {}
  
  /** Création des particules */
  virtual void createParticles();

  /** Mise à jour de la position des particules */
  virtual void updateParticlePosition();

 
};

#endif