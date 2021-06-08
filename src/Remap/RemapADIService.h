﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
#ifndef IREMAPADISERVICE_H
#define IREMAPADISERVICE_H

#include "TypesMahyco.h"

#include "Remap/IRemap.h"
#include "Remap/RemapADI_axl.h"
#include <arcane/ItemTypes.h>
#include "arcane/IMesh.h"
#include "arcane/IItemFamily.h"
#include "arcane/materials/IMeshMaterialMng.h"
#include "arcane/materials/IMeshMaterial.h"
#include "arcane/materials/IMeshEnvironment.h"
#include "arcane/materials/IMeshBlock.h"
#include "arcane/materials/MeshMaterialModifier.h"
#include "arcane/materials/MeshMaterialVariableRef.h"
#include "arcane/materials/MeshEnvironmentVariableRef.h"
#include "arcane/materials/MaterialVariableBuildInfo.h"
#include "arcane/materials/MeshBlockBuildInfo.h"
#include "arcane/materials/MeshEnvironmentBuildInfo.h"
#include "arcane/materials/MeshMaterialVariableDependInfo.h"
#include "arcane/materials/CellToAllEnvCellConverter.h"
#include "arcane/materials/MatCellVector.h"
#include "arcane/materials/EnvCellVector.h"
#include "arcane/materials/MatConcurrency.h"
#include "arcane/materials/MeshMaterialIndirectModifier.h"
#include "arcane/materials/MeshMaterialVariableSynchronizerList.h"
#include "arcane/materials/ComponentSimd.h"

#include "arcane/cea/ICartesianMesh.h"
#include "arcane/cea/CellDirectionMng.h"
#include "arcane/cea/FaceDirectionMng.h"
#include "arcane/cea/NodeDirectionMng.h"
#include "arcane/cea/CartesianConnectivity.h"

using namespace Arcane;
using namespace Arcane::Materials;

/**
 * Représente le service de Remap version ADI
 */
class RemapADIService 
: public ArcaneRemapADIObject
{
public:
  /** Constructeur de la classe */
  RemapADIService(const ServiceBuildInfo & sbi)
    : ArcaneRemapADIObject(sbi) {}
  
  /** Destructeur de la classe */
  virtual ~RemapADIService() {};
  
  struct interval {
    double inf, sup;
  };
  ICartesianMesh* m_cartesian_mesh;

public:
  /**
   * resize les variables du remap
   **/
  virtual void resizeRemapVariables(Integer nb_vars_to_project, Integer nb_env);
  /**
   * calcul des gradients aux faces
   **/
  virtual void computeGradPhiFace(Integer idir, String name, Integer nb_vars_to_project, Integer nb_env);
  /**
   * calcul des gradients aux faces ou flux aux faces 
   **/
  virtual void computeGradPhiCell(Integer idir, Integer nb_vars_to_project, Integer nb_env);
  /**
   * calcul des flux aux faces des cellules 
   **/
  virtual void computeUpwindFaceQuantitiesForProjection(Integer idir, String name, Integer nb_vars_to_project, Integer nb_env);
  /**
   * calcul des valeurs aux cellules 
   **/
  virtual void computeUremap(Integer idir, Integer nb_vars_to_project, Integer nb_env);
  /**
   * synchronisation des valeurs aux cellules 
   **/
  virtual void synchronizeUremap();
  /**
   * fonction pour la phase de projection duales
   **/
  virtual void computeDualUremap(Integer idir, String name, Integer nb_env);
  /**
   * synchronisation des valeurs aux noeuds 
   **/
  virtual void synchronizeDualUremap();
  /**
   * calcul le gradient aux mailles et reconstruction limitée d'une quantité Phi 
   **/
  virtual void computeAndLimitGradPhi(Integer projectionLimiterId, Face frontFace, Face backFace, 
                                      Cell cell, Cell frontcell, Cell backcell, int nb_vars) ;
  /**
   * calcul le gradient aux mailles et reconstruction limitée d'une quantité dual Phi Dual
   **/
  virtual void computeAndLimitGradPhiDual(Integer projectionLimiterId, 
                                          Node inode, Node frontnode, Node backnode,
                           Real3 grad_front, Real3 grad_back, Real h0, Real hplus, Real hmoins);
  /**
   * calcul le gradient aux mailles d'une quantité dual Phi Dual
   **/
  virtual void computeDualGradPhi(Node inode, Node frontfrontnode, Node frontnode, 
                                   Node backnode, Node backbacknode, Integer idir);

    /**
   * Fonction limiteur de gradient (maille regulier)
   **/
   virtual Real fluxLimiter(Integer projectionLimiterId, double r);
  /**
   * Fonction limiteur de gradient (maille irregulier)
   **/
   virtual Real fluxLimiterG(Integer projectionLimiterId, double gradplus,
                           double gradmoins, double y0, double yplus,
                           double ymoins, double h0, double hplus,
                           double hmoins);
  /**
   * Calcul des seuils de monotonie des reconstructions simple pente 
   **/
   virtual Real computeY0(Integer projectionLimiterId, double y0, double yplus,
                        double ymoins, double h0, double hplus, double hmoins,
                        Integer type);
   /**
   * Calcul des abscisses des pointes d'appui de la reconstuctions pente-borne
   **/
   virtual Real computexgxd(double y0, double yplus, double ymoins, double h0,
                          double y0plus, double y0moins, Integer type);
   /**
   * Calcul des oordonnes des pointes d'appui de la reconstuctions pente-borne
   **/
  
   virtual Real computeygyd(double y0, double yplus, double ymoins, double h0,
                          double y0plus, double y0moins, double grady,
                          Integer type);
  
  /**
   * Calcul des flux pente-borne (integration de  la reconstruction en trois morceaux) 
   * cas des mailles mixtes ou le flux de volume determine les flux de masse et d'energie
   **/
  virtual void computeFluxPP(Cell cell, Cell frontcell, Cell backcell, 
                                     Real face_normal_velocity, 
                                     Real deltat_n, Integer type, Real flux_threshold, 
                                     Integer projectionPenteBorneComplet, 
                                     Real dual_normal_velocity,
                                     Integer calcul_flux_dual,
                                     RealArrayView Flux, RealArrayView Flux_dual,
                                     int nbmat, int nb_vars);
  /**
   * Calcul des flux pente-borne (integration de  la reconstruction en trois morceaux) 
   * cas des mailles pures ou le flux de masse determine le flux d'energie
   **/
  virtual void computeFluxPPPure(Cell cell, Cell frontcell, Cell backcell, 
                                     Real face_normal_velocity, 
                                     Real deltat_n, Integer type, Real flux_threshold, 
                                     Integer projectionPenteBorneComplet, 
                                     Real dual_normal_velocity,
                                     Integer calcul_flux_dual,
                                     RealArrayView Flux, RealArrayView Flux_dual,
                                     int nbmat, int nb_vars);
  /**
   * Fonctions pour l'ordre 3
   **/
  virtual Real2 define_interval(double a, double b);
  virtual Real2 intersection(Real2 I1, Real2 I2);
  virtual double evaluate_grad(double hm, double h0, double hp, double ym, double y0,
                       double yp);
  virtual double evaluate_ystar(double hmm, double hm, double hp, double hpp,
                        double ymm, double ym, double yp, double ypp,
                        double gradm, double gradp);
  virtual double evaluate_fm(double x, double dx, double up, double du, double u6);
  
  virtual double evaluate_fp(double x, double dx, double um, double du, double u6);
  virtual double ComputeFluxOrdre3(double ymmm, double ymm, double ym, double yp,
                           double ypp, double yppp, double hmmm, double hmm,
                           double hm, double hp, double hpp, double hppp,
                           double v_dt);
  
  virtual Real INTY(double X, double x0, double y0, double x1, double y1);
  /**
   * calcul des flux d'une variable Phi final 
   **/
  virtual Real computeRemapFlux(Integer projectionOrder, Integer projectionAvecPlateauPente,
        Real face_normal_velocity, Real3 face_normal,
        Real face_length, Real phi_face,
        Real3 outer_face_normal, Real3 exy, Real deltat_n);
  
  Integer getOrdreProjection();
  bool hasProjectionPenteBorne();
  bool hasConservationEnergieTotale();    
  
private:
  
  Real m_arithmetic_thresold = 1.e-300;
  
};

#endif