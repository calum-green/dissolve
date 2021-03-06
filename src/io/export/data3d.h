// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "io/fileandformat.h"

// Forward Declarations
class Data3D;

// Data3D Export Formats
class Data3DExportFileFormat : public FileAndFormat
{
    public:
    // Available data formats
    enum Data3DExportFormat
    {
        BlockData3D,
        CartesianData3D,
        PDensData3D,
        nData3DExportFormats
    };
    Data3DExportFileFormat(std::string_view filename = "", Data3DExportFormat format = Data3DExportFileFormat::BlockData3D);

    /*
     * Format Access
     */
    private:
    // Return enum options for Data3DExportFormat
    static EnumOptions<Data3DExportFileFormat::Data3DExportFormat> data3DExportFormats();

    public:
    // Return number of available formats
    int nFormats() const;
    // Return format keyword for supplied index
    std::string_view formatKeyword(int id) const;
    // Return description string for supplied index
    std::string_view formatDescription(int id) const;
    // Return current format as Data3DExportFormat
    Data3DExportFormat data3DFormat() const;

    /*
     * Filename / Basename
     */
    public:
    // Return whether the file must exist
    bool fileMustExist() const { return false; }

    /*
     * Export Functions
     */
    private:
    // Export Data3D as simple block data
    bool exportBlock(LineParser &parser, const Data3D &data);
    // Export Data3D as cartesian data
    bool exportCartesian(LineParser &parser, const Data3D &data);
    // Export Data3D as pdens data
    bool exportPDens(LineParser &parser, const Data3D &data);

    public:
    // Export Data3D using current filename and format
    bool exportData(const Data3D &data);
};
