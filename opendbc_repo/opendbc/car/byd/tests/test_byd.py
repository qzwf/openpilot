from parameterized import parameterized

from opendbc.car import structs
from opendbc.car.byd.bydcan import byd_checksum
from opendbc.car.byd.fingerprints import FINGERPRINTS, FW_VERSIONS
from opendbc.car.byd.values import CAR, CarControllerParams, CanBus, BUTTONS, DBC, FW_QUERY_CONFIG, BYDPlatformConfig

CarParams = structs.CarParams
Ecu = CarParams.Ecu


class TestBYDFingerprint:
  @parameterized.expand(FINGERPRINTS.items())
  def test_can_fingerprints(self, car_model, fingerprints):
    assert len(fingerprints) > 0
    assert all(len(finger) for finger in fingerprints)

  @parameterized.expand(FINGERPRINTS.items())
  def test_fingerprint_keys_are_positive_integers(self, car_model, fingerprints):
    """All CAN address keys should be positive integers with valid DLC values."""
    for finger in fingerprints:
      for addr, dlc in finger.items():
        assert isinstance(addr, int) and addr > 0, f"Invalid CAN address: {addr}"
        assert isinstance(dlc, int) and dlc > 0, f"Invalid DLC for address {addr}: {dlc}"

  def test_no_fingerprint_overlap_between_variants(self):
    """Fingerprint sets for different platforms should not be identical."""
    all_fps = []
    for car_model, fingerprints in FINGERPRINTS.items():
      for fp in fingerprints:
        key_set = frozenset(fp.items())
        assert key_set not in all_fps, f"Duplicate fingerprint found in {car_model}"
        all_fps.append(key_set)


class TestBYDFirmware:
  def test_fw_versions_platforms_match_fingerprints(self):
    """Every platform in FW_VERSIONS should also be in FINGERPRINTS."""
    for car_model in FW_VERSIONS:
      assert car_model in FINGERPRINTS, f"{car_model} in FW_VERSIONS but missing from FINGERPRINTS"

  @parameterized.expand(FW_VERSIONS.items())
  def test_fw_versions_non_empty(self, car_model, fw_by_ecu):
    """Each platform should have at least one ECU with firmware versions."""
    assert len(fw_by_ecu) > 0
    for ecu_key, fws in fw_by_ecu.items():
      assert len(fws) > 0, f"Empty firmware list for {ecu_key}"
      for fw in fws:
        assert isinstance(fw, bytes) and len(fw) > 0, f"Invalid FW version: {fw}"

  def test_fw_versions_ecu_addresses(self):
    """ECU addresses should be valid CAN IDs and ECU tuples well-formed."""
    for car_model, fw_by_ecu in FW_VERSIONS.items():
      for (ecu, addr, subaddr), fws in fw_by_ecu.items():
        assert isinstance(addr, int) and addr > 0, f"Invalid ECU address: {addr}"
        assert subaddr is None or isinstance(subaddr, int), f"Invalid ECU subaddress: {subaddr}"

  def test_fw_query_config(self):
    """FW_QUERY_CONFIG should have at least one request."""
    assert len(FW_QUERY_CONFIG.requests) > 0
    for req in FW_QUERY_CONFIG.requests:
      assert len(req.request) > 0
      assert len(req.response) > 0


class TestBYDValues:
  def test_car_platform_exists(self):
    """BYD_ATTO3 should be defined as a platform."""
    assert hasattr(CAR, 'BYD_ATTO3')

  def test_car_platform_config(self):
    """Platform config should have valid car specs."""
    config = CAR.BYD_ATTO3.config
    assert isinstance(config, BYDPlatformConfig)
    assert config.specs.mass > 1000, "Mass should be reasonable for a car"
    assert config.specs.wheelbase > 2.0, "Wheelbase should be reasonable"
    assert config.specs.steerRatio > 10, "Steer ratio should be reasonable"
    assert config.specs.tireStiffnessFactor > 0, "Tire stiffness factor should be positive"

  def test_car_specs_values(self):
    """Spot-check specific BYD ATTO3 specs."""
    specs = CAR.BYD_ATTO3.config.specs
    assert specs.mass == 1750
    assert specs.wheelbase == 2.72
    assert specs.steerRatio == 14.8

  def test_dbc_mapping(self):
    """DBC map should contain an entry for BYD_ATTO3."""
    assert CAR.BYD_ATTO3 in DBC
    assert "pt" in DBC[CAR.BYD_ATTO3]
    assert DBC[CAR.BYD_ATTO3]["pt"] == "byd_general_pt"

  def test_can_bus_values(self):
    """CanBus should define expected bus IDs."""
    assert CanBus.ESC == 0
    assert CanBus.MRR == 1
    assert CanBus.MPC == 2
    assert CanBus.LOOPBACK == 128

  def test_buttons_defined(self):
    """BUTTONS list should be non-empty with valid structure."""
    assert len(BUTTONS) > 0
    for btn in BUTTONS:
      assert hasattr(btn, 'event_type')
      assert hasattr(btn, 'can_addr')
      assert hasattr(btn, 'can_msg')
      assert hasattr(btn, 'values')
      assert len(btn.values) > 0

  def test_button_event_types(self):
    """All button event types should be valid CarState.ButtonEvent.Type values."""
    expected_types = {
      structs.CarState.ButtonEvent.Type.leftBlinker,
      structs.CarState.ButtonEvent.Type.rightBlinker,
      structs.CarState.ButtonEvent.Type.accelCruise,
      structs.CarState.ButtonEvent.Type.decelCruise,
      structs.CarState.ButtonEvent.Type.cancel,
    }
    actual_types = {btn.event_type for btn in BUTTONS}
    assert actual_types == expected_types


class TestCarControllerParams:
  def test_steer_limits(self):
    """Steer limits should be positive and reasonable."""
    assert CarControllerParams.STEER_MAX > 0
    assert CarControllerParams.STEER_DELTA_UP > 0
    assert CarControllerParams.STEER_DELTA_DOWN > 0

  def test_driver_allowance(self):
    """Driver override parameters should be positive."""
    assert CarControllerParams.STEER_DRIVER_ALLOWANCE > 0
    assert CarControllerParams.STEER_DRIVER_MULTIPLIER > 0
    assert CarControllerParams.STEER_DRIVER_FACTOR > 0

  def test_steer_step(self):
    """STEER_STEP should be a small positive integer."""
    assert CarControllerParams.STEER_STEP > 0
    assert CarControllerParams.STEER_STEP <= 10

  def test_steer_error_max(self):
    """STEER_ERROR_MAX should be positive and less than STEER_MAX."""
    assert CarControllerParams.STEER_ERROR_MAX > 0
    assert CarControllerParams.STEER_ERROR_MAX < CarControllerParams.STEER_MAX

  def test_init(self):
    """CarControllerParams should be instantiable with a CP argument."""
    params = CarControllerParams(None)
    assert params is not None


class TestBYDChecksum:
  def test_checksum_zero_data(self):
    """Checksum of all-zero data should be deterministic."""
    result = byd_checksum(0xAF, bytes(8))
    assert isinstance(result, int)
    assert 0 <= result <= 0xFF

  def test_checksum_range(self):
    """Checksum should always produce a single byte result."""
    for key in (0x00, 0x7F, 0xAF, 0xFF):
      for data in (bytes(8), bytes([0xFF] * 8), bytes(range(8))):
        result = byd_checksum(key, data)
        assert 0 <= result <= 0xFF, f"Checksum out of range: {result}"

  def test_checksum_deterministic(self):
    """Same inputs should produce the same checksum."""
    data = bytes([0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80])
    result1 = byd_checksum(0xAF, data)
    result2 = byd_checksum(0xAF, data)
    assert result1 == result2

  def test_checksum_varies_with_key(self):
    """Different keys should generally produce different checksums."""
    data = bytes([0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80])
    results = {byd_checksum(key, data) for key in range(256)}
    # With 256 keys, we should get multiple distinct checksums
    assert len(results) > 1

  def test_checksum_varies_with_data(self):
    """Different data should generally produce different checksums."""
    results = set()
    for i in range(16):
      data = bytes([i] * 8)
      results.add(byd_checksum(0xAF, data))
    assert len(results) > 1

  def test_checksum_known_value(self):
    """Verify checksum against manually computed value.

    For byd_checksum(0xAF, bytes(8)):
      first_bytes_sum = 0 (all nibbles are 0)
      second_bytes_sum = 0
      remainder = 0
      second_bytes_sum += 0xAF >> 4 = 10
      first_bytes_sum += 0xAF & 0xF = 15
      first_part = ((-15 + 9) & 0xF) = (-6 & 0xF) = 10
      second_part = ((-10 + 9) & 0xF) = (-1 & 0xF) = 15
      result = (((10 + ((-0 + 5))) << 4) + 15) & 0xFF = ((15 << 4) + 15) & 0xFF = 0xFF = 255
    """
    assert byd_checksum(0xAF, bytes(8)) == 0xFF
