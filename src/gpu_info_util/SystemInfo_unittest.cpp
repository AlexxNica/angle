//
// Copyright 2017 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// SystemInfo_unittest.cpp: Unit tests for SystemInfo* helper functions.
//

#include "gpu_info_util/SystemInfo_internal.h"

#include <gtest/gtest.h>

using namespace angle;

namespace
{

// Test AMD Brahma driver version parsing
TEST(SystemInfoTest, AMDBrahmaVersionParsing)
{
    std::string version;

    // Check parsing fails when no version string is present.
    ASSERT_FALSE(ParseAMDBrahmaDriverVersion("I am a lumberjack.", &version));
    ASSERT_EQ("", version);

    // Check parsing when the string is just the version string, with and without dots
    ASSERT_TRUE(ParseAMDBrahmaDriverVersion("42", &version));
    ASSERT_EQ("42", version);
    ASSERT_TRUE(ParseAMDBrahmaDriverVersion("42.0.56", &version));
    ASSERT_EQ("42.0.56", version);

    // Check parsing with prefix / suffix
    ASSERT_TRUE(ParseAMDBrahmaDriverVersion("Version=42.0.56", &version));
    ASSERT_EQ("42.0.56", version);
    ASSERT_TRUE(ParseAMDBrahmaDriverVersion("42.0.56 is the version", &version));
    ASSERT_EQ("42.0.56", version);
    ASSERT_TRUE(ParseAMDBrahmaDriverVersion("42.0.56 is the version, 111", &version));
    ASSERT_EQ("42.0.56", version);
}

// Test AMD Catalyst version parsing
TEST(SystemInfoTest, AMDCatalystVersionParsing)
{
    std::string version;

    // Check parsing fails when no version string is present.
    ASSERT_FALSE(ParseAMDCatalystDriverVersion("I am a lumberjack.\nReleaseVersion=", &version));
    ASSERT_EQ("", version);

    // Check parsing fails when ReleaseVersion= is present but no number appears in the line
    ASSERT_FALSE(ParseAMDCatalystDriverVersion("11\nReleaseVersion=\n12", &version));
    ASSERT_EQ("", version);

    // Check parsing works on the simple case
    ASSERT_TRUE(ParseAMDCatalystDriverVersion("ReleaseVersion=42.0.56", &version));
    ASSERT_EQ("42.0.56", version);

    // Check parsing works if there are other lines
    ASSERT_TRUE(ParseAMDCatalystDriverVersion("11\nReleaseVersion=42.0.56\n12", &version));
    ASSERT_EQ("42.0.56", version);

    // Check parsing get the first version string
    ASSERT_TRUE(
        ParseAMDCatalystDriverVersion("ReleaseVersion=42.0.56\nReleaseVersion=0", &version));
    ASSERT_EQ("42.0.56", version);

    // Check parsing with prefix / suffix
    ASSERT_TRUE(ParseAMDCatalystDriverVersion("ReleaseVersion=version is 42.0.56", &version));
    ASSERT_EQ("42.0.56", version);
    ASSERT_TRUE(ParseAMDCatalystDriverVersion("ReleaseVersion=42.0.56 is the version", &version));
    ASSERT_EQ("42.0.56", version);
}
}
