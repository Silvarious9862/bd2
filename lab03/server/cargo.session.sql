SELECT employee.id,
       employee.badge,
       employee.lastname,
       employee.firstname,
       employee.experience,
       carriers.company_name AS carrier_name
FROM employee
JOIN carriers ON employee.carrier_id = carriers.id;
