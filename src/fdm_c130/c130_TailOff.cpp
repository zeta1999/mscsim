/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <fdm_c130/c130_TailOff.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_TailOff::C130_TailOff() :
    _ailerons     ( 0.0 ),
    _flaps        ( 0.0 ),
    _landing_gear ( 0.0 ),

    _dcl_dailerons ( 0.0 ),

    _dcx_dgear ( 0.0 ),
    _dcz_dgear ( 0.0 ),
    _dcm_dgear ( 0.0 )
{
    _dcx_dflaps = Table2D::createOneRecordTable( 0.0 );
    _dcz_dflaps = Table2D::createOneRecordTable( 0.0 );
    _dcm_dflaps = Table2D::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

C130_TailOff::~C130_TailOff() {}

////////////////////////////////////////////////////////////////////////////////

void C130_TailOff::readData( XmlNode &dataNode )
{
    //////////////////////////////
    TailOff::readData( dataNode );
    //////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcl_dailerons, "dcl_dailerons" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcx_dgear, "dcx_dgear" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcz_dgear, "dcz_dgear" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcm_dgear, "dcm_dgear" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcx_dflaps, "dcx_dflaps" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcz_dflaps, "dcz_dflaps" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcm_dflaps, "dcm_dflaps" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_TailOff::computeForceAndMoment( const Vector3 &vel_air_bas,
                                          const Vector3 &omg_air_bas,
                                          double airDensity,
                                          double ailerons,
                                          double flaps,
                                          double landing_gear )
{
    _ailerons     = ailerons;
    _flaps        = flaps;
    _landing_gear = landing_gear;

    ///////////////////////////////////////////////////////////////////////
    TailOff::computeForceAndMoment( vel_air_bas, omg_air_bas, airDensity );
    ///////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C130_TailOff::update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas )
{
    ////////////////////////////////////////////
    TailOff::update( vel_air_bas, omg_air_bas );
    ////////////////////////////////////////////

    Table cz_total = _cz + _dcz_dflaps.getTable( _flaps );

    _aoa_critical_neg = cz_total.getKeyOfValueMin();
    _aoa_critical_pos = cz_total.getKeyOfValueMax();
}

////////////////////////////////////////////////////////////////////////////////

double C130_TailOff::getCx( double angleOfAttack ) const
{
    return TailOff::getCx( angleOfAttack )
            + _dcx_dflaps.getValue( angleOfAttack, _flaps ) + _landing_gear * _dcx_dgear;
}

////////////////////////////////////////////////////////////////////////////////

double C130_TailOff::getCy( double sideslipAngle ) const
{
    return TailOff::getCy( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double C130_TailOff::getCz( double angleOfAttack ) const
{
    return TailOff::getCz( angleOfAttack )
            + _dcz_dflaps.getValue( angleOfAttack, _flaps ) + _landing_gear * _dcz_dgear;
}

////////////////////////////////////////////////////////////////////////////////

double C130_TailOff::getCl( double sideslipAngle ) const
{
    return TailOff::getCl( sideslipAngle ) + _ailerons * _dcl_dailerons;
}

////////////////////////////////////////////////////////////////////////////////

double C130_TailOff::getCm( double angleOfAttack ) const
{
    return TailOff::getCm( angleOfAttack )
            + _dcm_dflaps.getValue( angleOfAttack, _flaps ) + _landing_gear * _dcm_dgear;
}

////////////////////////////////////////////////////////////////////////////////

double C130_TailOff::getCn( double sideslipAngle ) const
{
    return TailOff::getCn( sideslipAngle );
}
