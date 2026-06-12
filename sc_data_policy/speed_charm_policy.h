/**
 * @file speed_charm_policy.h
 * @brief Common policy definitions for Speed Charm (sc) packages
 *
 * This header defines common policies shared by all Speed Charm (sc) packages.
 * It is designed for use with the core engine `speed_charmface_game_engine`
 * and its related modules.
 *
 * Covered topics:
 * - Return value and error code conventions
 * - File/chunk identification error rules
 * - Fixed size limits and constants
 * - Compression method identifiers
 * - Basic sc-prefixed type aliases
 *
 * The term "sc system" refers to the following packages:
 * - speed_charmface_game_engine (game engine core)
 * - speed_charm_3d (3D animation and object data)
 * - speed_charm_image (integrated image container)
 * - speed_charm_disk (engine-oriented data format)
 */

#ifndef _SPEED_CHARM_POLICY_H_
#define _SPEED_CHARM_POLICY_H_

/*====================================================
 * Basic return codes
 *====================================================*/

/**
 * @brief Normal successful result
 */
#define SC_POLICY_OUT_NORMAL 0

/**
 * @brief File open failure
 */
#define SC_POLICY_ERROR_FOPEN -1

/**
 * @brief Speed Charm contents or structure initialization failure
 */
#define SC_POLICY_ERROR_SC_INIT -2

/**
 * @brief Invalid or unsupported version
 */
#define SC_POLICY_ERROR_INVALID_VERSION -3

/**
 * @brief Memory allocation failure
 */
#define SC_POLICY_ERROR_ALLOC -4

/**
 * @brief End marker for file-related policy error range
 *
 * Alias of SC_POLICY_ERROR_ALLOC.
 */
#define SC_POLICY_FILE_ERROR_POLICY_END SC_POLICY_ERROR_ALLOC


/*====================================================
 * File identification / chunk errors
 *====================================================*/

/**
 * @brief Entry magic number mismatch
 */
#define SC_POLICY_ERROR_ENTRY_MAGIC_NUM -5

/**
 * @brief VERSION identifier mismatch
 */
#define SC_POLICY_ERROR_VERSION -6

/**
 * @brief USER identifier mismatch
 */
#define SC_POLICY_ERROR_USER -7

/**
 * @brief OS comment identifier mismatch
 */
#define SC_POLICY_ERROR_OS_COMMENT -8

/**
 * @brief Main comment identifier mismatch
 */
#define SC_POLICY_ERROR_COMMENT_MAIN -9

/**
 * @brief BEGIN identifier mismatch
 */
#define SC_POLICY_ERROR_BEG -10

/**
 * @brief END identifier mismatch
 */
#define SC_POLICY_ERROR_END -11

/**
 * @def SC_POLICY_ERROR_CHECK(error_code)
 * @brief Check whether an error code indicates failure
 *
 * @param error_code Error code to check.
 * @return Non-zero if @p error_code is a failure code.
 */
#define SC_POLICY_ERROR_CHECK(error_code) (error_code < SC_POLICY_OUT_NORMAL)

/**
 * @brief Generate additional identification error codes
 *
 * Generates a new negative error code by subtracting the given offset
 * from SC_POLICY_ERROR_END.
 *
 * @param offset Offset from SC_POLICY_ERROR_END
 * @return Generated identification error code
 *
 * Example:
 * - offset = 1 -> -12
 * - offset = 2 -> -13
 */
#define SC_POLICY_ERROR_IDENTIFY_START(offset) (SC_POLICY_ERROR_END - offset)


/*====================================================
 * OS string / comment size rules
 *====================================================*/

/**
 * @brief Fixed size of OS identification string
 */
#define SC_POLICY_OS_STR_SIZE 5

/**
 * @brief Calculate available comment size excluding OS string
 *
 * @param max_comment_size Total comment buffer size
 * @return Usable comment size excluding OS string area
 */
#define SC_POLICY_OS_ANOTHER_COMMENT_SIZE(max_comment_size) \
    (max_comment_size - SC_POLICY_OS_STR_SIZE)


/*====================================================
 * Lock key policy
 *====================================================*/

/**
 * @brief Lock key ID indicating no encryption / no lock
 */
#define SC_POLICY_NO_LOCK_KEY 0


/*====================================================
 * File name constraints
 *====================================================*/

/**
 * @brief Maximum file name buffer size
 */
#define SC_POLICY_FILE_NAME_MAX (1 << 5)

/**
 * @brief Maximum size of file format suffix buffer (including terminator)
 */
#define SC_POLICY_FILE_FORMAT_MAX ((1 << 2) + 1)

/**
 * @def SC_POLICY_FILE_FORMAT_NAME(low)
 * @brief Generate standard file format string literal with "sc" prefix
 *
 * @param low Lower format suffix (e.g., "i" -> ".sci").
 * @return String literal in the form ".sc<low>".
 */
#define SC_POLICY_FILE_FORMAT_NAME(low) ".""sc" #low

/*====================================================
 * Compression method identifiers
 *====================================================*/

/**
 * @brief Zstandard compression method ID
 */
#define SC_ZSTD_COMPRESS_ID 0

/**
 * @brief ZIP compression method ID
 */
#define SC_ZIP_COMPRESS_ID 1

/**
 * @brief 7ZIP compression method ID
 */
#define SC_7ZIP_COMPRESS_ID 2

/**
 * @brief Required begin chunk identifier for sc-system file formats.
 *
 * This chunk marks the start boundary of the main data region in sc-system
 * container formats (e.g., SCI). Readers should validate this identifier
 * to ensure the file structure begins correctly.
 *
 * Typical use:
 * - Written after header/version information.
 * - Verified during read to detect corrupted or malformed files.
 */
#define SC_POLICY_REQUIRE_BEG_CHUNK "BEG"

/**
 * @brief Required end chunk identifier for sc-system file formats.
 *
 * This chunk marks the end boundary of the main data region in sc-system
 * container formats (e.g., SCI). Readers should validate this identifier
 * to ensure the file was fully written and is structurally complete.
 *
 * Typical use:
 * - Written as the final marker.
 * - Verified during read to detect truncated/incomplete files.
 */
#define SC_POLICY_REQUIRE_END_CHUNK "END"

/**
 * @def SC_POLICY_REQUIRE_SC_ENTRY_MAGIC_NUM(sc_magic_num)
 * @brief Policy macro for entry magic number requirement token
 *
 * @param sc_magic_num Magic number token/value.
 * @return Expanded magic number token/value.
 */

#define SC_POLICY_REQUIRE_SC_ENTRY_MAGIC_NUM(sc_magic_num) sc_magic_num
/**
 * @def SC_POLICY_REQUIRE_SC_ENTRY_MAGIC_NUM_POS(pos)
 * @brief Policy macro to require magic number position at entry start
 *
 * @param pos Position of the entry magic number.
 * @return Non-zero if the magic number position equals zero.
 *
 * @details
 * This policy enforces that the sc-system entry magic number
 * must be located at position 0 (the beginning of the entry or file).
 * It defines an absolute placement rule and does not specify
 * the magic number value, size, or format.
 */
#define SC_POLICY_REQUIRE_SC_ENTRY_MAGIC_NUM_POS(pos) (pos == 0)

/**
 * @def SC_POLICY_REQUIRE_CHECK_END_NEXT(end_chunk_next)
 * @brief Policy macro to check that END chunk is followed by EOF
 *
 * @param end_chunk_next Next read result after END chunk.
 * @return Non-zero if @p end_chunk_next equals EOF.
 */
#define SC_POLICY_REQUIRE_CHECK_END_NEXT(end_chunk_next) (end_chunk_next == EOF)

/**
 * @def SC_POLICY_BEG_CHUNK_POS(magic_num_pos, beg_pos, end_pos)
 * @brief Policy macro to validate ordering of magic/begin/end positions
 *
 * @param magic_num_pos Magic number position.
 * @param beg_pos Begin chunk position.
 * @param end_pos End chunk position.
 * @return Non-zero if positions satisfy ordering constraint.
 */
#define SC_POLICY_BEG_CHUNK_POS(magic_num_pos, beg_pos, end_pos) (magic_num_pos < beg_pos && beg_pos < end_pos)

/**
 * @def SC_POLICY_REQUIRE_VERSION_BLOCK(ver)
 * @brief Policy macro for required version block token
 *
 * @param ver Version token/value.
 * @return Expanded version token/value.
 */
#define SC_POLICY_REQUIRE_VERSION_BLOCK(ver) ver

/**
 * @def SC_POLICY_REQUIRE_USER_BLOCK(usr)
 * @brief Policy macro for required user block token
 *
 * @param usr User token/value.
 * @return Expanded user token/value.
 */
#define SC_POLICY_REQUIRE_USER_BLOCK(usr) usr

/**
 * @def SC_POLICY_REQUIRE_COMMENT_BLOCK(com)
 * @brief Policy macro for required comment block token
 *
 * @param com Comment token/value.
 * @return Expanded comment token/value.
 */
#define SC_POLICY_REQUIRE_COMMENT_BLOCK(com) com


/*====================================================
 * sc-prefixed basic type aliases
 *====================================================*/

/**
 * @brief sc 64-bit unsigned integer type
 */
typedef uint64_t sc_size;

/**
 * @brief sc 64-bit unsigned integer type
 */
typedef uint64_t sc_uint64;

/**
 * @brief sc 32-bit unsigned integer type
 */
typedef uint32_t sc_uint;

/**
 * @brief sc 16-bit unsigned integer type
 */
typedef uint16_t sc_ushort;

/**
 * @brief sc 8-bit unsigned integer type
 */
typedef uint8_t sc_ubyte;

/**
 * @brief sc 64-bit signed integer type
 */
typedef int64_t sc_int64;

/**
 * @brief sc 32-bit signed integer type
 */
typedef int32_t sc_int;

/**
 * @brief sc 16-bit signed integer type
 */
typedef int16_t sc_short;

/**
 * @brief sc 8-bit signed integer type
 */
typedef char sc_byte;

/**
 * @brief sc FILE Pointer Type
 */
typedef FILE * SC_FILE;

/**
 * @brief sc error code base type
 */
typedef long sc_err_t;

/**
 * @brief Bit width of sc unsigned unit used in version packing
 */
#define SC_POLICY_SC_UINT_8_BITS 8

/**
 * @brief Policy macro to set sc version token/value
 *
 * @param ver Version token/value.
 * @return Expanded version token/value.
 */
#define SC_POLICY_SET_SC_VERSION(ver) (ver)

/**
 * @brief Base major version (lowest supported major)
 */
#define SC_POLICY_BASE_MAJOR_VERSION SC_POLICY_SET_SC_VERSION(1u)

/**
 * @brief Base minor version (lowest supported minor)
 */
#define SC_POLICY_BASE_MINOR_VERSION SC_POLICY_SET_SC_VERSION(0u)

/**
 * @brief Pack major/minor into a single version value
 *
 * @param major Major version.
 * @param minor Minor version.
 * @return Packed version value.
 */
#define SC_POLICY_SET_SC_VERSIONS(major, minor) ((major << SC_POLICY_SC_UINT_8_BITS) | minor)

/**
 * @brief Base packed version (lowest supported version)
 */
#define SC_POLICY_BASE_VERSION SC_POLICY_SET_SC_VERSIONS(SC_POLICY_BASE_MAJOR_VERSION, SC_POLICY_BASE_MINOR_VERSION)

/**
 * @brief check that version is equal or more than the lowest version
 */
#define SC_POLICY_CHECK_BASE_VERSION(ver) (ver >= SC_POLICY_BASE_VERSION)
#define SC_POLICY_GET_MAJOR(ver) (ver >> SC_POLICY_SC_UINT_8_BITS)

#define SC_POLICY_CHECK_SUPPORT_VERSION(src_ver, now_ver) ((src_ver >> SC_POLICY_SC_UINT_8_BITS) <= (now_ver >> SC_POLICY_SC_UINT_8_BITS))
#define SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(src_ver, module_ver) ((src_ver >> SC_POLICY_SC_UINT_8_BITS) >= (module_ver >> SC_POLICY_SC_UINT_8_BITS))

#define SC_POLICY_SET_NOW_VERSION(ver) ver

/**
 * @brief Temporary preprocessor policy flags
 *
 * These macros are used only to let the preprocessor
 * detect whether a policy flag is defined.
 *
 * @details
 * The flags are valid only between `#define` and `#undef`.
 * After being undefined, they leave no effect on subsequent code.
 * They do not represent runtime states and are intended strictly
 * for conditional compilation checks.
 *
 * Usage example:
 * @code
 * #define SC_POLICY_DEBUG
 *
 * #ifdef SC_POLICY_DEBUG
 *     // debug-only code
 * #endif
 *
 * #undef SC_POLICY_DEBUG
 * @endcode
 *
 * `SC_POLICY_RELEASE` follows the same usage pattern.
 */
#define SC_POLICY_DEBUG
#define SC_POLICY_RELEASE
#undef SC_POLICY_DEBUG
#undef SC_POLICY_RELEASE

/**
 * @def SC_POLICY_SPEED_CHARMFACE_ENGINE_NAME
 * @brief Logical identifier for the Speed Charm core engine
 *
 * Defines the canonical token name of the Speed Charm face engine.
 * This macro represents the engine identity at the policy level.
 *
 * @details
 * This macro expands to an identifier, not a string.
 * It is intended for use in:
 * - Project-wide engine identification
 * - Macro composition and token generation
 * - Compile-time identification and naming conventions
 *
 * This definition does not imply any implementation, behavior,
 * or runtime semantics. It exists purely as a policy-level identifier.
 */
#define SC_POLICY_SPEED_CHARMFACE_ENGINE_NAME SPEED_CHARMFACE_ENGINE

/**
 * @def SC_POLICY_SPEED_CHARM_PROJECT_NAME(format)
 * @brief Generates a Speed Charm project identifier with standard prefix
 *
 * Concatenates the token `SPEED_CHARM_` with the specified format identifier
 * to produce a canonical Speed Charm project name.
 *
 * @param format
 * Identifier appended to the `SPEED_CHARM_` prefix.
 *
 * @return
 * A preprocessor token in the form `SPEED_CHARM_<format>`.
 *
 * @details
 * - Uses preprocessor token concatenation (`##`)
 * - Produces an identifier, not a string literal
 * - Does not create symbols, objects, or runtime behavior
 *
 * Example:
 * @code
 * SC_POLICY_SPEED_CHARM_PROJECT_NAME(3D)
 * // Expands to: SC3
 * @endcode
 *
 * This macro exists solely to enforce naming consistency
 * across Speed Charm projects.
 */
#define SC_POLICY_SPEED_CHARM_PROJECT_NAME(format) SPEED_CHARM_ ## format

/**
 * @brief This macro must not be wrapped by other macros
 *
 * #define XXX#### 1 SC_POLICY_FINAL__
 *
 * invalid code
 * X #define XXXX### XXX###
 */
#define SC_POLICY_FINAL__

/**
 * @def SC_POLICY_SC_PROJECT_NAME(format)
 * @brief Generates a short internal project identifier with `sc` prefix
 *
 * Concatenates the token `sc` with the specified format identifier
 * to produce a compact, internal-use project name.
 *
 * @param format
 * Identifier appended to the `sc` prefix.
 *
 * @return
 * A preprocessor token in the form `sc<format>`.
 *
 * @details
 * - Intended for internal or shorthand usage
 * - Produces a token, not a string
 * - Does not define semantics, behavior, or structure
 *
 * Example:
 * @code
 * SC_POLICY_SC_PROJECT_NAME(3d)
 * // Expands to: sc3d
 * @endcode
 *
 * In accordance with sc_policy design principles,
 * this macro defines naming rules only and imposes
 * no behavioral or structural constraints.
 */
#define SC_POLICY_SC_PROJECT_NAME(format) sc ## format

/**
 * @def sc_policy_si_strap
 * @brief Policy macro for declaring static inline functions
 *
 * Expands to `static inline` and is intended to be used when defining
 * small, header-only helper functions that belong to the sc_policy layer.
 *
 * @details
 * This macro serves the following purposes:
 * - Enforces a consistent function declaration style within sc_policy
 * - Prevents multiple-definition linker errors across translation units
 * - Encourages compile-time inlining for lightweight utility functions
 *
 * The use of this macro implies:
 * - The function has internal linkage (`static`)
 * - The function is suitable for inlining (`inline`)
 * - The function does not represent core logic or algorithms
 *
 * @note
 * This macro defines *only* a declaration convention.
 * It does not define behavior, implementation rules, or semantics.
 *
 * @warning
 * Functions declared with this macro are expected to be:
 * - Side-effect minimal
 * - Independent of runtime state
 * - Safe to include in multiple translation units
 *
 * Example:
 * @code
 * sc_policy_si_strap sc_uint sc_policy_example_func(sc_uint v) {
 *     return v;
 * }
 * @endcode
 *
 * In accordance with sc_policy design principles, this macro exists
 * solely to standardize declaration style and does not impose any
 * execution or structural guarantees.
 */
#define sc_policy_si_strap static inline


/**
 * @file sc_loose_policy.h
 * @brief Optional I/O helper policy for Speed Charm (sc) packages
 *
 * @details
 * ## Status and requirement level
 * - **sc_policy is mandatory** for the Speed Charm system.
 * - **sc_loose_policy is NOT mandatory**.
 *
 * sc_loose_policy exists only as a convenience layer to unify
 * lightweight file I/O call patterns (open/close/read/write/position).
 * Any sc-system implementation may choose not to use sc_loose_policy
 * without violating sc_policy requirements.
 *
 * ## Scope
 * This header defines:
 * - Boolean-style wrappers for standard C file I/O operations
 * - Short aliases (`scl_*`) for concise call sites
 *
 * This header does NOT define:
 * - File format layouts or chunk structures
 * - Algorithms or data processing behavior
 * - Error-code mapping rules (e.g., which SC_POLICY_ERROR_* to return)
 * - EOF vs I/O error distinction policies
 *
 * ## Notes
 * The macros are intentionally "loose":
 * - Read/write macros primarily report whether the requested count was satisfied.
 * - Detailed diagnostics (e.g., `feof`, `ferror`) remain the caller's responsibility.
 */

#ifndef _SC_LOOSE_POLICY_H_
#define _SC_LOOSE_POLICY_H_

#if defined(_MSVC_VER_)

/**
 * @brief Internal fopen wrapper for MSVC (uses fopen_s)
 */
#define scfopen__(fp, f_path, f_mode) (fopen_s(&fp, f_path, f_mode) != 0)

#ifdef SC_LOOSE_POLICY_64_FILE

/**
 * @brief Internal tell wrapper (64-bit) for MSVC
 */
#define scftell__ _ftelli64
/**
 * @brief Internal seek wrapper (64-bit) for MSVC
 */
#define scfseek__ _fseeki64
#else
/**
 * @brief Internal tell wrapper for MSVC
 */
#define scftell__ ftell
/**
 * @brief Internal seek wrapper for MSVC
 */
#define scfseek__ fseek
#endif

#else

/**
 * @brief Internal fopen wrapper for non-MSVC platforms (uses fopen)
 */
#define scfopen__(fp, f_path, f_mode) ((fp = fopen(f_path, f_mode)) == NULL)

#ifdef SC_LOOSE_POLICY_64_FILE

/**
 * @brief Enable 64-bit file offsets
 */
#define _FILE_OFFSET_BITS 64
/**
 * @brief Internal tell wrapper (64-bit) for non-MSVC platforms
 */
#define scftell__ ftello
/**
 * @brief Internal seek wrapper (64-bit) for non-MSVC platforms
 */
#define scftell__ fseeko
#else

/**
 * @brief Internal tell wrapper for non-MSVC platforms
 */
#define scftell__ ftell
/**
 * @brief Internal seek wrapper for non-MSVC platforms
 */
#define scfseek__ fseek
#endif

#endif

/**
 * @brief Internal close wrapper
 */
#define scfclose__ fclose
/**
 * @brief Internal write wrapper
 */
#define scfwrite__ fwrite
/**
 * @brief Internal read wrapper
 */
#define scfread__ fread

/**
 * @brief Internal file size offset type
 */
typedef sc_int64 scfsize_t____;
/**
 * @brief Public alias for file size offset type
 */
typedef scfsize_t____ scf_size_t;

/**
 * @def sc_loose_policy_fopen(fp, f_path, f_mode)
 * @brief Open a file and assign the resulting FILE pointer.
 *
 * @details
 * Attempts to open the file specified by @p f_path with mode @p f_mode,
 * assigns the resulting pointer to @p fp, and returns whether the open
 * operation succeeded.
 *
 * @param fp     L-value FILE pointer variable to receive the result.
 * @param f_path File path.
 * @param f_mode Open mode string (e.g., "rb", "wb").
 *
 * @return
 * - `true` if the file was opened successfully (`fp != NULL`)
 * - `false` otherwise
 *
 * @note This macro is optional. Not using it does not violate sc_policy.
 */
#define sc_loose_policy_fopen(fp, f_path, f_mode) scfopen__(fp, f_path, f_mode)

/**
 * @def sc_loose_policy_fclose(fp)
 * @brief Close a file and nullify the pointer.
 *
 * @details
 * Calls `fclose(fp)` and then sets @p fp to `NULL` to clearly invalidate it.
 *
 * @param fp L-value FILE pointer variable to be closed and nullified.
 *
 * @note This macro is optional. Not using it does not violate sc_policy.
 */
#define sc_loose_policy_fclose(fp) (fp = (fp ? (scfclose__(fp), NULL) :  NULL))

/**
 * @def sc_loose_policy_get_fpos(fp)
 * @brief Get current file position.
 *
 * @param fp FILE pointer.
 * @return The return value of `ftell(fp)`.
 *
 * @note This macro is optional. Not using it does not violate sc_policy.
 */
#define sc_loose_policy_get_fpos(fp) scftell__(fp)

/**
 * @def sc_loose_policy_fwrite(fp, buf, type, num)
 * @brief Write elements to a file and report full completion.
 *
 * @details
 * Writes @p num elements of type @p type from @p buf into @p fp and returns
 * whether all requested elements were written.
 *
 * @param fp   FILE pointer.
 * @param buf  Source buffer.
 * @param type Element type (used only for `sizeof(type)`).
 * @param num  Number of elements to write.
 *
 * @return
 * - `true` if `fwrite(...) == num`
 * - `false` otherwise
 *
 * @note This macro is optional. Not using it does not violate sc_policy.
 */
#define sc_loose_policy_fwrite(fp, buf, type, num) (scfwrite__(buf, sizeof(type), num, fp) == num)

/**
 * @def sc_loose_policy_fread(fp, buf, type, num)
 * @brief Read elements from a file and report full completion.
 *
 * @details
 * Reads @p num elements of type @p type into @p buf from @p fp and returns
 * whether all requested elements were read.
 *
 * This macro does not distinguish EOF from I/O errors; callers may use
 * `feof(fp)` and `ferror(fp)` if needed.
 *
 * @param fp   FILE pointer.
 * @param buf  Destination buffer.
 * @param type Element type (used only for `sizeof(type)`).
 * @param num  Number of elements to read.
 *
 * @return
 * - `true` if `fread(...) == num`
 * - `false` otherwise
 *
 * @note This macro is optional. Not using it does not violate sc_policy.
 */
#define sc_loose_policy_fread(fp, buf, type, num) (scfread__(buf, sizeof(type), num, fp) == num)

/**
 * @def sc_loose_policy_seek(fp, off, opt)
 * @brief Seek within a file using the platform policy seek function
 *
 * @param fp File pointer.
 * @param off Offset.
 * @param opt Seek origin (e.g., SEEK_SET).
 * @return Return value of the underlying seek function.
 */
#define sc_loose_policy_seek(fp, off, opt) scfseek__(fp, off, opt)

/*====================================================
 * Short aliases (optional convenience names)
 *====================================================*/

/**
 * @def scl_fopen
 * @brief Alias of sc_loose_policy_fopen (optional convenience name).
 */
#define scl_fopen sc_loose_policy_fopen

/**
 * @def scl_close
 * @brief Alias of sc_loose_policy_fclose (optional convenience name).
 */
#define scl_close sc_loose_policy_fclose

/**
 * @def scl_get_fpos
 * @brief Alias of sc_loose_policy_get_fpos (optional convenience name).
 */
#define scl_get_fpos sc_loose_policy_get_fpos

/**
 * @def scl_fwrite
 * @brief Alias of sc_loose_policy_fwrite (optional convenience name).
 */
#define scl_fwrite sc_loose_policy_fwrite

/**
 * @def scl_fread
 * @brief Alias of sc_loose_policy_fread (optional convenience name).
 */
#define scl_fread sc_loose_policy_fread

/**
 * @def scl_fgoto
 * @brief Alias of sc_loose_policy_seek (optional convenience name).
 */
#define scl_fgoto sc_loose_policy_seek

/**
 * @def scl_fgoto_e(fp)
 * @brief Seek to end of file (optional convenience name).
 *
 * @param fp File pointer.
 * @return Return value of the underlying seek function.
 */
#define scl_fgoto_e(fp) sc_loose_policy_seek(fp, 0, SEEK_END)

#ifdef _EXT_DEBUGGER_H_
/**
 * @brief Debugger package alias used by sc_loose_policy when available
 */
typedef Ext_Debugger_Package Scl_Dp;
/**
 * @brief Set debugger information (optional integration)
 */
#define SC_LOOSE_POLICY_DEBUGGER(error_code, literal_log) set_debug_information(error_code, literal_log)

/**
 * @brief Put a debugger line break / normal output marker (optional integration)
 */
#define SC_LOOSE_POLICY_DP_PUT(dp) dp_puts_ln(dp, SC_POLICY_OUT_NORMAL)

#endif
#endif /* _SC_LOOSE_POLICY_H_ */
/**
 * @page sc_policy_layers Policy layers: policy / require / loose_policy
 *
 * @section sc_policy_layer_policy sc_policy (common policy)
 * sc_policy defines the **common rules and shared vocabulary** for the sc-system.
 *
 * It provides:
 * - Common return values and error code conventions (SC_POLICY_OUT_*, SC_POLICY_ERROR_*)
 * - Shared identifiers and constants (e.g., BEG/END identifiers, size limits)
 * - Shared version baseline and comparison rule (SC_POLICY_BASE_VERSION, SC_POLICY_CHECK_BASE_VERSION)
 * - Shared error range generation rule (SC_POLICY_ERROR_IDENTIFY_START)
 *
 * sc_policy does not mandate a specific file layout implementation.
 * It exists to ensure that independently implemented sc-system modules
 * use the same meanings for values, identifiers, and errors.
 *
 * @section sc_policy_layer_require sc_require policy (required rules)
 * sc_require policy refers to **required checks / required conditions** that
 * sc-system implementations must satisfy.
 *
 * Notes:
 * - These requirements may be expressed using SC_POLICY_REQUIRE_* macros,
 *   but using those macros is **not mandatory**.
 * - The mandatory part is the **behavior** (the required validation and rules),
 *   not the macro usage.
 *
 * Examples of requirement expressions:
 * - Required boundary identifiers (BEG/END) must be validated where applicable.
 * - Required end condition (END must be followed by EOF) must be validated where applicable.
 * - Required minimum version rule must be enforced where applicable.
 *
 * The SC_POLICY_REQUIRE_* macros exist as policy-level tokens to make required
 * rules explicit at call sites, but they do not impose runtime semantics by
 * themselves beyond what the implementation performs.
 *
 * @section sc_policy_layer_loose sc_loose_policy (optional convenience layer)
 * sc_loose_policy is an **optional** helper layer.
 *
 * It provides:
 * - Lightweight wrappers for standard C file I/O calls (open/close/read/write/seek/tell)
 * - Short aliases (scl_*) for concise call sites
 *
 * sc_loose_policy does not define:
 * - File format layouts or chunk structures
 * - Algorithms or data processing behavior
 * - Error-code mapping rules
 *
 * Not using sc_loose_policy does not violate sc_policy requirements.
 */

#endif

